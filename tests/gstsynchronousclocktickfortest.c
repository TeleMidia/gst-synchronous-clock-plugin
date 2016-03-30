#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <gio/gio.h>
#include <gstsynchronousclock.h>
#include <inttypes.h>
#include <gst/audio/gstaudiobasesink.h>

#define MS_TO_NS 1000000
  
GstElement *pipeline;
GstElement *decodebin;
GstElement *playsink;
GstElement *alsasink;
GMainLoop *loop;
GCancellable *cancellable;

guint freq = 15;

static gpointer
main_loop_thread (gpointer _)
{
  g_main_loop_run (loop);
  return NULL;
}


static void
cb_pad_added (GstElement *dec,
        GstPad     *pad,
        gpointer    data)
{
  GstCaps *caps;
  GstStructure *str;
  const gchar *name;
  GstPadTemplate *templ;
  GstElementClass *klass;

  /* check media type */
  caps = gst_pad_query_caps (pad, NULL);
  str = gst_caps_get_structure (caps, 0);
  name = gst_structure_get_name (str);

  klass = GST_ELEMENT_GET_CLASS (playsink);

  if (g_str_has_prefix (name, "audio")) 
    templ = gst_element_class_get_pad_template (klass, "audio_sink");
  else if (g_str_has_prefix (name, "video"))
    templ = gst_element_class_get_pad_template (klass, "video_sink");
  else if (g_str_has_prefix (name, "text")) 
    templ = gst_element_class_get_pad_template (klass, "text_sink");
  else 
    templ = NULL;

  if (templ) 
  {
    GstPad *sinkpad;

    sinkpad = gst_element_request_pad (playsink, templ, NULL, NULL);

    if (!gst_pad_is_linked (sinkpad))
      gst_pad_link (pad, sinkpad);

    gst_object_unref (sinkpad);
  }
}


static gboolean 
bus_cb (GstBus *bus, GstMessage *message, gpointer user_data)
{
  switch (GST_MESSAGE_TYPE (message))
  {
    case GST_MESSAGE_ERROR: 
    {
      GError *err;
      gchar *debug;

      gst_message_parse_error (message, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);

      g_cancellable_cancel (cancellable);
      g_main_loop_quit (loop);
      break;
    }
    case GST_MESSAGE_EOS:
    {
      /* end-of-stream */
      g_print ("End-of-stream");
      g_cancellable_cancel (cancellable);
      g_main_loop_quit (loop);
      break;
    }
    default:
      /* unhandled message */
      break;
  }

  return TRUE;
}

int 
main(int argc, char *argv[])
{
  GstBus *bus;
  GstClock *clock, *audioclock;
  gint64 dur;
  cancellable = g_cancellable_new ();

  if (argc < 2)
  {
    printf ("Usage: %s <uri>\n", argv[0]);
    return EXIT_SUCCESS;
  }
  loop = g_main_loop_new (NULL, FALSE);
  
  gst_init (&argc, &argv);
  
  pipeline = gst_pipeline_new ("pipeline");
  decodebin = gst_element_factory_make ("uridecodebin", "decoder");
  playsink = gst_element_factory_make ("playsink", "sink");
  alsasink = gst_element_factory_make ("alsasink", "audiosink");

  g_assert (pipeline);
  g_assert (decodebin);
  g_assert (playsink);
  g_assert (alsasink);

  bus = gst_pipeline_get_bus (GST_PIPELINE(pipeline));
  gst_bus_add_watch (bus, bus_cb, NULL);
  gst_object_unref (bus);

  g_object_set (G_OBJECT (decodebin), "uri", argv[1], NULL);
  g_object_set (G_OBJECT (playsink), "audio-sink", alsasink, NULL);
  g_object_set (G_OBJECT (alsasink), "slave-method", 
      GST_AUDIO_BASE_SINK_SLAVE_SKEW, NULL);
  
  GST_OBJECT_FLAG_SET (G_OBJECT(alsasink), GST_CLOCK_FLAG_CAN_SET_MASTER);

  gst_bin_add_many (GST_BIN (pipeline), decodebin, playsink, NULL);
  
  g_signal_connect (decodebin, "pad-added", G_CALLBACK (cb_pad_added), NULL);

  clock = gst_synchronous_clock_new ();

  gst_pipeline_use_clock (GST_PIPELINE(pipeline), clock);
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  gst_element_get_state (pipeline, NULL, NULL, GST_CLOCK_TIME_NONE);

  gst_element_query_duration (decodebin, GST_FORMAT_TIME, &dur);

  /* Setting 'clock' as master of 'audioclock' */
  audioclock = GST_AUDIO_BASE_SINK (alsasink)->provided_clock;
  gst_clock_set_timeout (audioclock, freq * MS_TO_NS);

  if (!gst_clock_set_master (audioclock, clock))
    printf ("Could not slave audioclock (%p) to clock (%p)\n", 
        (void *) audioclock, (void *) clock);
 
  g_thread_unref (g_thread_new ("main-loop", main_loop_thread, NULL));
  gst_synchronous_clock_tick_for (clock, (uint64_t)dur, cancellable);

  if (GST_STATE (pipeline) != GST_STATE_NULL)
    gst_element_set_state (pipeline, GST_STATE_NULL);

  g_object_unref (clock);
  g_main_loop_unref (loop);
  gst_object_unref (pipeline);
  g_object_unref (cancellable);


  return EXIT_SUCCESS;
}
