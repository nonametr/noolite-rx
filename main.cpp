/*
 *  Copyright 2015 Igor K. aka nonametr
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <libusb-1.0/libusb.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <signal.h>

using namespace std;

#include "common.h"
#include "rx2164.h"
#include "loger.h"

static string cfg_path;
static string app_name;
static volatile bool running = true;

/**
 * @brief strToInt converts string to int
 * @param str string to convert
 * @return uint
 */
uint str_to_int(const char *str)
{
    uint result;
    sscanf(str, "%u", &result);
    return result;
}

/**
 * @brief rm_pid removes pid file
 * @param app_name - path to pid file
 */
void rm_pid(string app_name)
{
    app_name += ".pid";
    unlink(app_name.c_str());
}

/**
 * @brief write_pid writes pid file
 * @param app_name - path to pid file
 */
void write_pid(string app_name)
{
    uint32_t pid;

    app_name += ".pid";
    FILE* fPid = fopen(app_name.c_str(), "w");
    if (fPid)
    {
        pid = getpid();
        fprintf(fPid, "%u", (uint32_t)pid);
        fclose(fPid);
    }
}

/**
 * @brief terminateSignalHandler - OS signal handler, unlocks mutex
 * @param sigNum - signal id
 */
static void terminate_signal_handler(int sigNum)
{
    traceNotice("Received SIGNAL %d", sigNum);

    running = false;
}

/**
 * @brief setupSignals - prepare to handle OS signals
 */
void setup_signals()
{
    signal(SIGUSR1, terminate_signal_handler);
    signal(SIGTERM, terminate_signal_handler);

    signal(SIGCHLD, SIG_DFL);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
}

/**
 * @brief daemonize fork process and init service(daemon)
 * @param appName
 */
void daemonize()
{
    /* Step 1: Forking the Parent Process */
    pid_t pid = fork();
    if (pid < 0)
    {
        traceError("fork() fails in daemonize()\n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Step 2: Changing the File Mode Mask */
    umask(0);

    /* Step 3: Creating a Unique Session ID */
    pid_t sid = setsid();
    if (sid < 0)
    {
        traceError("setsid() fails in daemonize()");
        exit(EXIT_FAILURE);
    }

    /* Step 4: Changing the Working Directory */
    if (chdir("/") < 0)
    {
        traceError("chdir(\"/\") fails in daemonize()");
        exit(EXIT_FAILURE);
    }

    /* Step 5: Closing Standard File Descriptors */
    if (close(STDIN_FILENO) < 0)
    {
        traceError("close(STDIN_FILENO) fails in daemonize()");
    }
    if (close(STDOUT_FILENO) < 0)
    {
        traceError("close(STDOUT_FILENO) fails in daemonize()");
    }
    if (close(STDERR_FILENO) < 0)
    {
        traceError("close(STDERR_FILENO) fails in daemonize()");
    }

    /* Step 6: Setup signals */
    setup_signals();

    /* Step 7: Write new PID to file */
    write_pid(app_name);
}

/**
 * @brief print_help - prints help message
 */
void print_help()
{
    traceNotice("Usage: %s [OPTIONS]\n", app_name.c_str());
    traceNotice("  -h | --help               Print this message\n");
    traceNotice("  -d | --daemonize          Run as device pooling daemon(service)\n");
    traceNotice("  -c | --config_path $path  Set configuration file\n");
    traceNotice("  -b | --bind_ch $ch        Bind channel\n");
    traceNotice("  --unbind_ch $ch           Unbind channel\n");
    traceNotice("  --unbind_all              Unbind all channels\n");
    traceNotice("  --loop                    Run interactive device pooling\n");
}

void parse_args( int argc, char **argv )
{
    ASSERT_WITH_CODE(argc > 1, "Wrong command parameters, type -h | --help for help message", return);

    int opt;
    int option_index;

    enum { OPT_DAEMONIZE = 1000, OPT_HELP, OPT_CONFIG, OPT_BIND, OPT_UNBIND, OPT_UNBIND_ALL, OPT_LOOP};

    struct option optionList[] = {
        {"daemonize", no_argument, 0, OPT_DAEMONIZE},
        {"help", no_argument, 0, OPT_HELP},
        {"config_path", required_argument, 0, OPT_CONFIG},
        {"bind_ch", required_argument, 0, OPT_BIND},
        {"unbind_ch", required_argument, 0, OPT_UNBIND},
        {"unbind_all", no_argument, 0, OPT_UNBIND_ALL},
        {"loop", no_argument, 0, OPT_LOOP},
        {0, 0, 0, 0}
    };

    app_name = argv[0];
    while ((opt = getopt_long(argc, argv, "dhcb", optionList, &option_index )) != -1)
    {
        switch (opt)
        {
        case OPT_DAEMONIZE:
        case 'd':
        {
            traceNotice("Daemonizing...\r");
            daemonize();
            RX2164 rx;

            ASSERT_WITH_CODE(rx.open() == RX2164_STATE::OPENED, "Failed to open RX2164", return);
            ASSERT_WITH_CODE(rx.start() == RX2164_STATE::LOOPING, "Failed to start quering loop on RX2164", return);

            while(running)
                sleep(1);

            rx.close();
        }
            break;
        case 'c':
        case OPT_CONFIG:
            cfg_path = string(argv[optind-1]);
            break;
        case 'h':
        case OPT_HELP:
            print_help();
            exit(EXIT_SUCCESS);
        case 'b':
        case OPT_BIND:
        {
            int ch = str_to_int(argv[optind-1]);
            RX2164 rx;

            ASSERT_WITH_CODE(rx.open() == RX2164_STATE::OPENED, "Failed to open RX2164", return);
            ASSERT_WITH_CODE(rx.start() == RX2164_STATE::LOOPING, "Failed to start quering loop on RX2164", return);

            rx.bindChannel(ch);
            rx.close();
        }
            break;
        case OPT_UNBIND:
        {
            int ch = str_to_int(argv[optind-1]);
            RX2164 rx;

            ASSERT_WITH_CODE(rx.open() == RX2164_STATE::OPENED, "Failed to open RX2164", return);
            ASSERT_WITH_CODE(rx.start() == RX2164_STATE::LOOPING, "Failed to start quering loop on RX2164", return);

            rx.unbindChannel(ch);
            rx.close();
        }
            break;
        case OPT_UNBIND_ALL:
        {
            RX2164 rx;

            ASSERT_WITH_CODE(rx.open() == RX2164_STATE::OPENED, "Failed to open RX2164", return);
            ASSERT_WITH_CODE(rx.start() == RX2164_STATE::LOOPING, "Failed to start quering loop on RX2164", return);

            rx.unbindAll();
            rx.close();
        }
            break;
        case OPT_LOOP:
        {
            RX2164 rx;

            ASSERT_WITH_CODE(rx.open() == RX2164_STATE::OPENED, "Failed to open RX2164", return);
            ASSERT_WITH_CODE(rx.start() == RX2164_STATE::LOOPING, "Failed to start quering loop on RX2164", return);

            traceNotice("Enter 'q' to exit\n");

            char ch;

            do
            {
                std::cin >> ch;
            } while(ch != 'q');

            traceNotice("Stopping...\n");

            rx.close();
        }
            break;
        default:
            traceNotice("Wrong command parameters, type -h | --help for help message\n");
            exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char** argv)
{
    ASSERT_WITH_CODE(!libusb_init(nullptr), "Can't init. libusb", return 1);

    write_pid(argv[0]);
    openlog(argv[0], LOG_PID | LOG_PERROR, LOG_USER);
    parse_args(argc, argv);
    libusb_exit(nullptr);
    rm_pid(app_name);

    return EXIT_SUCCESS;
}

