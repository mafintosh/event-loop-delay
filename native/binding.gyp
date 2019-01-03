{
  "targets": [{
    "target_name": "event_loop_delay",
    "include_dirs": [
      "<!(node -e \"require('napi-macros')\")"
    ],
    "sources": [
      "binding.c"
    ],
    'cflags': [
      '-O3'
    ]
  }]
}

