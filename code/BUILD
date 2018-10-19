cc_library(
    name = "task_queue",
    srcs = ["task_queue.cc"],
    hdrs = ["task_queue.h"],
)

cc_library(
    name = "master_and_slave",
    srcs = [
        "master.cc",
        "slave.cc",
        "task_queue.cc",
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
