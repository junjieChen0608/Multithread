cc_library(
    name = "task_queue",
    hdrs = ["task_queue.h"],
)

cc_library(
    name = "master_and_slave",
    srcs = [
        "master.cc",
        "slave.cc",
    ],
    hdrs = [
        "master.h",
        "slave.h",
        "task_queue.h",
    ],
)

cc_binary(
    name = "multithread",
    srcs = ["main.cc"],
    linkopts = ["-pthread"],
    deps = [
        ":master_and_slave",
    ],
)
