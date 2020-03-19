/* This is a simple HTTP(S) web server much like Python's SimpleHTTPServer */

#include <App.h>

#include <chrono>
#include <random>

/* Helpers for this example */
#include "uWebSockets/examples/helpers/AsyncFileReader.h"
#include "uWebSockets/examples/helpers/AsyncFileStreamer.h"
#include "uWebSockets/examples/helpers/Middleware.h"

/* optparse */
#define OPTPARSE_IMPLEMENTATION
#include "uWebSockets/examples/helpers/optparse.h"

int main(int argc, char **argv) {

    int option;
    struct optparse options;
    optparse_init(&options, argv);

    struct optparse_long longopts[] = {
        {"port", 'p', OPTPARSE_REQUIRED},
        {"help", 'h', OPTPARSE_NONE},
        {"passphrase", 'a', OPTPARSE_REQUIRED},
        {"key", 'k', OPTPARSE_REQUIRED},
        {"cert", 'c', OPTPARSE_REQUIRED},
        {"dh_params", 'd', OPTPARSE_REQUIRED},
        {0}
    };

    int port = 3000;
    struct us_socket_context_options_t ssl_options = {};

    while ((option = optparse_long(&options, longopts, nullptr)) != -1) {
        switch (option) {
        case 'p':
            port = atoi(options.optarg);
            break;
        case 'a':
            ssl_options.passphrase = options.optarg;
            break;
        case 'c':
            ssl_options.cert_file_name = options.optarg;
            break;
        case 'k':
            ssl_options.key_file_name = options.optarg;
            break;
        case 'd':
            ssl_options.dh_params_file_name = options.optarg;
            break;
        case 'h':
        case '?':
            fail:
            std::cout << "Usage: " << argv[0] << " [--help] [--port <port>] [--key <ssl key>] [--cert <ssl cert>] [--passphrase <ssl key passphrase>] [--dh_params <ssl dh params file>] <public root>" << std::endl;
            return 0;
        }
    }

    char *root = optparse_arg(&options);
    if (!root) {
        goto fail;
    }

    AsyncFileStreamer asyncFileStreamer(root);

    constexpr unsigned kCardsInDeck{52};

    struct PerSocketData
    {
        PerSocketData()
        {
            for (uint8_t i=0; i<kCardsInDeck; ++i) { mDeck[i] = i; }
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(mDeck.begin(), mDeck.end(), std::default_random_engine(seed));
            std::cout << "Deck shuffled\n";
        }

        std::array<uint8_t, kCardsInDeck> mDeck;
    };

    uWS::TemplatedApp<false>::WebSocketBehavior behavior{
        .open = [](auto *ws, auto *req) {
            /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
        },
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            ws->send(message, opCode);
        },
        .close = [](auto *ws, int code, std::string_view message) {
            /* You may access ws->getUserData() here */
        }
    };



    uWS::App()

    // Serve HTTP
    .get("/*", [&asyncFileStreamer](auto *res, auto *req) {
        serveFile(res, req);
        asyncFileStreamer.streamFile(res, req->getUrl());
    })

    // Serve WebSocket API
    .ws<PerSocketData>("/*", {
        /* Settings */
        .compression = uWS::DISABLED,
        .maxPayloadLength = 16 * 1024,
        .idleTimeout = 120,
        .maxBackpressure = 1 * 1024 * 1204,
        /* Handlers */
        .open = [](auto *ws, auto *req) {
            /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
        },
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            ws->send(message, opCode);
        },
        .drain = [](auto *ws) {
            /* Check ws->getBufferedAmount() here */
        },
        .ping = [](auto *ws) {
            /* Not implemented yet */
        },
        .pong = [](auto *ws) {
            /* Not implemented yet */
        },
        .close = [](auto *ws, int code, std::string_view message) {
            /* You may access ws->getUserData() here */
        }
    })

    .listen(port, [port, root](auto *token) {
        if (token) {
            std::cout << "Serving " << root << " over HTTP a " << port << std::endl;
        }
    })

    .run();

    std::cout << "Failed to listen to port " << port << std::endl;
}
