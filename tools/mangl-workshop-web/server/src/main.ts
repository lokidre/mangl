// process.env.UV_THREADPOOL_SIZE =
//     Math.ceil(Math.max(4, require('os').cpus().length * 1.5));

import http from 'http';

import './logger';
import './config';
import './app';
// import './api';

// import {dbInitConfig} from './tsback/mongodb/mongodb_init';

process.on('uncaughtException', (err) => {
    console.error('Uncaught error', err);
    process.exit(1); //mandatory (as per the Node.js docs)
});

export default async function main() {
    let port = config_.server.port;

    try {
        expressApp_.set('port', port);

        // dbInitConfig(config_.db);

        const server = http.createServer(expressApp_);

        function onServerListening() {
            log_.info('Listenting On:', port);
        }

        server.listen(port, onServerListening);
    } catch (err) {
        log_.error(err);
        process.exit(1);
    }

    return 0;
}
