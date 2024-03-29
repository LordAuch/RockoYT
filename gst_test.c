#include <gst/gst.h>
#include <string.h>
#include <stdio.h>




typedef struct _CustomData {
  gboolean is_live;
  GstElement *pipeline;
  GMainLoop *loop;
} CustomData;

static void cb_message (GstBus *bus, GstMessage *msg, CustomData *data) {

  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;

      gst_message_parse_error (msg, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);

      gst_element_set_state (data->pipeline, GST_STATE_READY);
      g_main_loop_quit (data->loop);
      break;
    }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      gst_element_set_state (data->pipeline, GST_STATE_READY);
      g_main_loop_quit (data->loop);
      break;
    case GST_MESSAGE_BUFFERING: {
      gint percent = 0;

      /* If the stream is live, we do not care about buffering. */
      if (data->is_live) break;

      //break; //This Avoid the pipeline!!

      gst_message_parse_buffering (msg, &percent);
      g_print ("Buffering (%3d%%)\r", percent);
      /* Wait until buffering is complete before start/resume playing */
      
      if (percent < 10)
        gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
      else
        gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
      break;
    }
    case GST_MESSAGE_CLOCK_LOST:
      /* Get a new clock */
      gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
      gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
      break;
    default:
      /* Unhandled message */
      break;
    }
}

int main(int argc, char *argv[]) {
  GstElement *pipeline;
  GstBus *bus;
  GstStateChangeReturn ret;
  GMainLoop *main_loop;
  CustomData data;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Initialize our data structure */
  memset (&data, 0, sizeof (data));

  /* Build the pipeline */
  pipeline = gst_parse_launch ("playbin uri=https://rr4---sn-9gv7zn76.googlevideo.com/videoplayback?expire=1708045649&ei=8WDOZeX_Ltbgir4Pk-m3gAw&ip=168.195.202.11&id=o-AIT1v6H8Di3NaKAnxbUee1omptYtfPvm8fFjnEGS7ujV&itag=139&source=youtube&requiressl=yes&xpc=EgVo2aDSNQ%3D%3D&mh=uK&mm=31%2C26&mn=sn-9gv7zn76%2Csn-q4flrnl7&ms=au%2Conr&mv=m&mvi=4&pl=22&gcr=mx&initcwndbps=452500&vprv=1&mime=audio%2Fmp4&gir=yes&clen=710095&dur=116.160&lmt=1693248791517906&mt=1708023699&fvip=3&keepalive=yes&fexp=24007246&c=ANDROID_MUSIC&txp=2318224&sparams=expire%2Cei%2Cip%2Cid%2Citag%2Csource%2Crequiressl%2Cxpc%2Cgcr%2Cvprv%2Cmime%2Cgir%2Cclen%2Cdur%2Clmt&sig=AJfQdSswRQIhAMVF1t1xBLZaJxYK7FB-2GqH3qDnfcyoCRwtdM881u8OAiAzj9o9dFoTJERR2ZazfiLNUUo2f1YLFL7nvD8TOM80eA%3D%3D&lsparams=mh%2Cmm%2Cmn%2Cms%2Cmv%2Cmvi%2Cpl%2Cinitcwndbps&lsig=APTiJQcwRQIgNc4qEcbG5TRC0VDlYYwRCeKCnMS4drmMSEB9e8-6gpICIQCBXj5Bwfkc3Ct2OUvHjqnP4XA60g3gbBIOuu7qkMgjlg%3D%3D", NULL);
  bus = gst_element_get_bus (pipeline);

  /* Start playing */
  ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (pipeline);
    return -1;
  } else if (ret == GST_STATE_CHANGE_NO_PREROLL) {
    data.is_live = TRUE;
  }

  main_loop = g_main_loop_new (NULL, FALSE);
  data.loop = main_loop;
  data.pipeline = pipeline;

  gst_bus_add_signal_watch (bus);
  g_signal_connect (bus, "message", G_CALLBACK (cb_message), &data);

  g_main_loop_run (main_loop);

  /* Free resources */
  g_main_loop_unref (main_loop);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  return 0;
}
