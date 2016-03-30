#include <gst/gst.h>
#include <gstsynchronousclock.h>

#define SLEEP 2000000

int main(int argc, char *argv[])
{
  GstElement *pipeline;
  GstElement *fakesrc;
  GstElement *fakesink;
  GstClock *clock, *tmpclock;

  gst_init (&argc, &argv);

  pipeline = gst_pipeline_new ("pipeline");
  fakesrc = gst_element_factory_make ("fakesrc", "fakesrc");
  fakesink = gst_element_factory_make ("fakesink", "fakesink");
  clock = gst_synchronous_clock_new ();

  g_assert (pipeline);
  g_assert (fakesrc);
  g_assert (fakesink);
  g_assert (clock);

  gst_bin_add_many (GST_BIN (pipeline), fakesrc, fakesink, NULL);
  gst_element_link (fakesrc, fakesink);
  gst_pipeline_use_clock (GST_PIPELINE (pipeline), clock);

  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  gst_synchronous_clock_tick_for (clock, 1000);
  tmpclock = gst_pipeline_get_pipeline_clock(GST_PIPELINE (pipeline));
  g_assert (gst_clock_get_time (tmpclock) == 1000);
  g_object_unref (tmpclock);
  
  gst_synchronous_clock_tick_for (clock, 1000);
  tmpclock = gst_pipeline_get_pipeline_clock(GST_PIPELINE (pipeline));
  g_assert (gst_clock_get_time (tmpclock) == 2000);
  g_object_unref (tmpclock);
  
  g_usleep (1000000);

  tmpclock = gst_pipeline_get_pipeline_clock(GST_PIPELINE (pipeline));
  g_assert (gst_clock_get_time (tmpclock) == 2000);
  g_object_unref (tmpclock);

  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  g_object_unref (clock);
  return 0;
}
