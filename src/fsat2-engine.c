/* SPDX-License-Identifier: LGPL-2.1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>
#include <assert.h>
#include <pthread.h>

const char *sub_str = "tcp://0.0.0.0:2808";
const char *pub_str = "tcp://0.0.0.0:2809";

static void *capture_task(void *ctx)
{
	int ret;

	printf("Capture/logging task listening on %s\n", sub_str);

	/* Subscriber (RX) */
	void *subscriber = zmq_socket(ctx, ZMQ_SUB);
	ret = zmq_connect(subscriber, pub_str);
	if (ret < 0) {
		perror("Unable to connect");
		exit(1);
	}
	ret = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
	if (ret < 0) {
		perror("Failed to call setsockopt");
		exit(1);
	}

	while (1) {
		zmq_msg_t msg;
		zmq_msg_init_size(&msg, 8192);

		/* Receive data */
		if (zmq_msg_recv(&msg, subscriber, 0) < 0) {
			zmq_msg_close(&msg);
			printf("ZMQ Err: %s\n", zmq_strerror(zmq_errno()));
			continue;
		}

		size_t datalen = zmq_msg_size(&msg);

		uint8_t *rx_data = zmq_msg_data(&msg);

		printf("XSUB: ");

		for (size_t i = 0; i < datalen; ++i) {
			printf("%#X, ", rx_data[i]);
		}
		printf("\n\r");

		zmq_msg_close(&msg);
	}
}

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

	assert(zmq_bind(xsub, sub_str) == 0);

	void *xpub = zmq_socket(ctx, ZMQ_XPUB);

	if (!xpub) {
		fprintf(stderr, "Failed to create XPUB socket! Exiting...");
		exit(EXIT_FAILURE);
	}

	assert(zmq_bind(xpub, pub_str) == 0);

	fprintf(stdout, "Started FlatSat2 Engine Proxy - xsub[*:2808] - xpub[*:2809]\n");

	pthread_t capworker;
	pthread_create(&capworker, NULL, capture_task, ctx);

	zmq_proxy(xsub, xpub, NULL);

	zmq_ctx_destroy(ctx);

	return EXIT_SUCCESS;
}
