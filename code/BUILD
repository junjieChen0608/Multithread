cc_library(
    name = "TaskQueue",
    hdrs = ["TaskQueue.h"],
)

cc_library(
    name = "MasterAndSlave",
    srcs = [
        "Master.cc",
        "Slave.cc",
    ],
    hdrs = [
        "Master.h",
        "Slave.h",
        "TaskQueue.h",
    ],
)

cc_binary(
    name = "multithread",
    srcs = ["main.cc"],
    linkopts = ["-pthread"],
    deps = [
        ":MasterAndSlave",
    ],
)
