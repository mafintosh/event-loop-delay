#include <node_api.h>
#include <napi-macros.h>
#include <uv.h>

#define DELAY_TIMER_RESOLUTION 10
#define DELAY_TIMER_RESOLUTION_NS DELAY_TIMER_RESOLUTION * 1e6
#define DELAY_TIMER_THRESHOLD_NS 5e6 + DELAY_TIMER_RESOLUTION_NS

typedef struct {
  uint32_t delay;
  uint32_t times;
  uint64_t prev;
  uv_timer_t timer;
  napi_ref buffer;
  napi_env env;
} delay_timer_t;

static void on_uv_interval (uv_timer_t *req) {
  delay_timer_t *delay = (delay_timer_t *) req->data;
  uint64_t time = uv_hrtime();
  uint64_t delta = time - delay->prev;

  if (delta > DELAY_TIMER_THRESHOLD_NS) {
    delta = (delta - DELAY_TIMER_RESOLUTION_NS) / 1e6;
    if (delta > 0xffffffff) delta = 0xffffffff;
    delay->delay += (uint32_t) delta;
    delay->times++;
  }

  delay->prev = time;
}

static void on_uv_close (uv_handle_t *handle) {
  delay_timer_t *self = (delay_timer_t *) handle->data;
  napi_delete_reference(self->env, self->buffer);
}

NAPI_METHOD(stop_delay_timer) {
  NAPI_ARGV(1)
  NAPI_ARGV_BUFFER_CAST(delay_timer_t *, delay, 0)

  uv_timer_stop(&(delay->timer));
  uv_close((uv_handle_t *) &(delay->timer), on_uv_close);

  return NULL;
}

NAPI_METHOD(start_delay_timer) {
  NAPI_ARGV(1)
  NAPI_ARGV_BUFFER_CAST(delay_timer_t *, delay, 0)

  uv_timer_init(uv_default_loop(), &(delay->timer));
  napi_create_reference(env, argv[0], 1, &(delay->buffer));

  delay->env = env;
  delay->prev = uv_hrtime();
  delay->timer.data = delay;

  uv_timer_start(&(delay->timer), on_uv_interval, DELAY_TIMER_RESOLUTION, DELAY_TIMER_RESOLUTION);
  uv_unref((uv_handle_t *) &(delay->timer));

  return NULL;
}

NAPI_INIT () {
  NAPI_EXPORT_FUNCTION(start_delay_timer)
  NAPI_EXPORT_FUNCTION(stop_delay_timer)
  NAPI_EXPORT_SIZEOF(delay_timer_t)
}
