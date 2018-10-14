cc_library(
    name = "TaskQueue",
    srcs = ["TaskQueue.cc"],
    hdrs = ["TaskQueue.h"],
)

cc_library(
    name = "Slave",
    srcs = ["Slave.cc"],
    hdrs = ["Slave.h"],
)

cc_library(
    name = "Master",
    srcs = ["Master.cc"],
    hdrs = ["Master.h"],
)

cc_binary(
    name = "multithread",
    srcs = ["main.cc"],
    deps = [
        ":Master",
        ":Slave",
        ":TaskQueue",
    ],
)
