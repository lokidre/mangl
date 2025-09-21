import express from 'express';
import * as BodyParser from 'body-parser';

import * as RequestIp from 'request-ip';

//import * as multer from 'multer'

global.expressApp_ = express();

const app = global.expressApp_;

//app.use(express.urlencoded())

//
// Obtaining caller ip address
//
app.set('trust proxy', true);
app.use(RequestIp.mw());

//
// Body parser
//
//app.use(BodyParser.json());
app.use(BodyParser.raw({type: 'application/octet-stream', limit: '50mb'}));
app.use(BodyParser.raw({type: 'image/jpeg', limit: '50mb'}));
app.use(BodyParser.raw({type: 'image/png', limit: '50mb'}));
app.use(express.json());
//app.use(express.text({type: 'text/html'}))

//app.use(BodyParser.urlencoded({extended: true}))

//
// Dev - allow cors
//
if (config_.dev) {
    log_.info('Enabling CORS...');

    app.use((req, res, next) => {
        res.header('Access-Control-Allow-Origin', '*');
        res.header(
            'Access-Control-Allow-Headers',
            'Origin, X-Requested-With, Content-Type, Accept, Authorization, Range'
        );
        next();
    });
}
