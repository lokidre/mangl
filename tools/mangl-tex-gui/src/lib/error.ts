
// Creates 'Error' object from any object
export function makeError(err: any): Error {
    if (err instanceof Error)
        return err;


    if (typeof err === 'string')
        return new Error(err);

    return new Error(`Unknown error type: ${err}`);
}
