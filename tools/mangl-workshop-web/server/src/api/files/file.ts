import { getReqParam, requireQueryParam } from '../../tsback/req/req_params';
import { registerHandlerSession } from '../../tsback/req/req_register';
import { respondJsonData } from '../../tsback/req/req_response';

registerHandlerSession('files', 'fetch', async (req, res, session) => {
   
    let file = {
        name: 'asd',
        date: '12312',
        tab: 2,
        file: 'asdasda'
    }

    respondJsonData(res, { file: file });
});


