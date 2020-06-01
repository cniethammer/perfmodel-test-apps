/* StreamApp simulates the behaviour of a memory bound parallel application.
 * Serial execution time is taken as a representative for the amount of data
 * to be processes.  The data to be processed is distrubuted evenly accross
 * processes.  Each processes is assumed to run on a single core.  Each core
 * has a bandwidth limit.  Further, the overall combined bandwidth of all cores
 * cannot exceed a given maximal bandwidth.  Default values mimic an Intel
 * Skylake multi core Xeon server CPU.
 * StreamApp is run as a MPI program.
 *
 * Copyright (c) 2020       Christoph Niethammer <niethammer@hlrs.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <mpi.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <thread>

#include "cmdline.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int comm_size = 1;
    int comm_rank = 0;
    MPI_Comm_size(comm, &comm_size);
    MPI_Comm_rank(comm, &comm_rank);

    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0) {
        exit(1);
    }
    if (args.help_given) {
        if (comm_rank == 0) {
            cmdline_parser_print_help();
        }
        MPI_Finalize();
        return 0;
    }

    double t1 = args.t_serial_arg;
    double b = args.b_serial_arg;
    double b_max = args.b_max_arg;
    double np = static_cast<double>(comm_size);

    std::chrono::duration<double> sleeptime((t1 / np) /
                                            std::min(b, b_max / np));
    printf("rank %d going to sleep for %lf seconds ...\n", comm_rank,
           sleeptime.count());
    std::this_thread::sleep_for(sleeptime);
    printf("rank %d finished ...\n", comm_rank);
    MPI_Finalize();
    return 0;
}
