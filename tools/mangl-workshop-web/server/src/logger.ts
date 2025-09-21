import {Logger} from 'tslog';

global.log_ = new Logger({
    displayDateTime: false,
    displayLoggerName: false,
    exposeErrorCodeFrame: false,
});


// global.log_ = new Logger({
//     prettyLogTemplate: "{{logLevelName}}\t[{{filePathWithLine}}{{name}}]\t",
//     argumentsArrayName: "argumentsArray",
// });

