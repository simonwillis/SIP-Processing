#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <uv.h>

int main() {
    auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    auto stdlogger = std::make_shared<spdlog::logger>("stdlogger", sink);
    spdlog::register_logger(stdlogger);

    auto logger = spdlog::get("stdlogger");
    logger->info("Starting Control App");

    // This is intended to be a simple control interface that can be driven by cmd line initially

    logger->info("Control App Stopped");
    return 0;
}