/* SPDX-License-Identifier: LGPL-2.1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>
#include <assert.h>
#include <pthread.h>

int main(int argc, char **argv)
{
	void *ctx = zmq_ctx_new();

	if (!ctx) {
		fprintf(stderr, "Failed to create ZMQ context! Exiting...");
		exit(EXIT_FAILURE);
	}

	void *xsub = zmq_socket(ctx, ZMQ_XSUB);

	if (!xsub) {
		fprintf(stderr, "Failed to create XSUB socket! Exiting...");
		exit(EXIT_FAILURE);
	}

	assert(zmq_bind(xsub, "tcp://*:2808") == 0);

	void *xpub = zmq_socket(ctx, ZMQ_XPUB);

	if (!xpub) {
		fprintf(stderr, "Failed to create XPUB socket! Exiting...");
		exit(EXIT_FAILURE);
	}

	assert(zmq_bind(xpub, "tcp://*:2809") == 0);

	fprintf(stdout, "Started FlatSat2 Engine Proxy - xsub[*:2808] - xpub[*:2809]\n");

	zmq_proxy(xsub, xpub, NULL);

	zmq_ctx_destroy(ctx);

	return EXIT_SUCCESS;
}
