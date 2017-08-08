/*
 * @class PulseSink
 *
 * @author: Kacper Patro patro.kacper@gmail.com
 * @date May 13, 2015
 *
 * @version 1.0 beta
 * @copyright Copyright (c) 2015 Kacper Patro
 * @par License
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "pulse_sink.h"

GstPadProbeReturn PulseSinkCallbacks::UnlinkCallPulseSink(GstPad *pad, GstPadProbeInfo *info, gpointer container_ptr) {
    PulseSink::Data *data = reinterpret_cast<PulseSink::Data *>(container_ptr);

    GstPad *sinkpad;

    if(!g_atomic_int_compare_and_exchange(&data->removing, FALSE, TRUE))
        return GST_PAD_PROBE_OK;

    sinkpad = gst_element_get_static_pad(data->queue, "sink");
    gst_pad_unlink(data->teepad, sinkpad);
    gst_object_unref(sinkpad);

    gst_bin_remove(GST_BIN(data->player_data->pipeline), data->queue);
    gst_bin_remove(GST_BIN(data->player_data->pipeline), data->sink);

    gst_element_set_state(data->sink, GST_STATE_NULL);
    gst_element_set_state(data->queue, GST_STATE_NULL);

    gst_object_unref(data->sink);
    gst_object_unref(data->queue);

    gst_element_release_request_pad(data->player_data->tee, data->teepad);
    gst_object_unref(data->teepad);

    data->linked = false;

    return GST_PAD_PROBE_REMOVE;
}

PulseSink::PulseSink():
    data_(new Data) {
        data_->abstract_sink = this;
        data_->linked = false;
        data_->player_data = NULL;
    }

PulseSink::~PulseSink() {
    delete data_;
}

void PulseSink::InitSink(void *player_data) {
    if(linked())
        return;

    data_->player_data = reinterpret_cast<Player::Data *>(player_data);

    GstPad *sinkpad;
    GstPadTemplate *templ;

    templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(data_->player_data->tee), "src_%u");
    data_->teepad = gst_element_request_pad(data_->player_data->tee, templ, NULL, NULL);

    char buff[100];

    strcpy(buff, name());
    strcat(buff, "_queue");

    data_->queue = gst_element_factory_make("queue", buff);
    g_assert(data_->queue);

    g_signal_connect(data_->queue, "underrun", G_CALLBACK(PulseSinkCallbacks::QueueUnderrun), data_->player_data->pipeline);
    g_signal_connect(data_->queue, "pushing", G_CALLBACK(PulseSinkCallbacks::QueuePushing), data_->player_data->pipeline);

    strcpy(buff, name());
    strcat(buff, "_sink");

    data_->sink = gst_element_factory_make("pulsesink", buff);
    g_assert(data_->sink);

    g_object_set(data_->sink, "async", TRUE, NULL);

    data_->removing = FALSE;

    gst_object_ref(data_->queue);
    gst_object_ref(data_->sink);

    gst_bin_add_many(GST_BIN(data_->player_data->pipeline),
            data_->queue,
            data_->sink,
            NULL);

    g_assert(gst_element_link_many(
                data_->queue,
                data_->sink,
                NULL)
            );

    gst_element_sync_state_with_parent(data_->queue);
    gst_element_sync_state_with_parent(data_->sink);

    sinkpad = gst_element_get_static_pad(data_->queue, "sink");
    gst_pad_link(data_->teepad, sinkpad);
    gst_object_unref(sinkpad);

    data_->linked = true;
}

const char *PulseSink::name() const {
    return "pulse_sink";
}

void PulseSink::Finish() {
    if(!linked()) {
        return;
    }

    gst_pad_add_probe(data_->teepad, GST_PAD_PROBE_TYPE_IDLE, PulseSinkCallbacks::UnlinkCallPulseSink, data_, NULL);
}

bool PulseSink::linked() const {
    return data_->linked;
}

void PulseSinkCallbacks::QueueUnderrun(GstElement *queue, gpointer pipeline) {
    g_assert(gst_element_set_state(reinterpret_cast<GstElement *>(pipeline), GST_STATE_PAUSED));
}

void PulseSinkCallbacks::QueuePushing(GstElement *queue, gpointer pipeline) {
    g_assert(gst_element_set_state(reinterpret_cast<GstElement *>(pipeline), GST_STATE_PLAYING));
}
