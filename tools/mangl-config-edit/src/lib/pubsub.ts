interface IHandler {
    type: string
    func: any
}

export class PubSub {
    // Initialization
    release() {
        this.listeners_.forEach((l) => this.remove(l.type, l.func))
        this.listeners_.clear()
    }

    //
    // Generic document events
    //
    on(type: string, listener: any) {
        document.addEventListener(type, listener)
    }

    remove(type: string, listener: any) {
        document.removeEventListener(type, listener)
    }

    dispatchCustomEvent<T>(ev: CustomEvent<T>) {
        document.dispatchEvent(ev)
    }

    dispatch<T>(type: string, detail?: T) {
        this.dispatchCustomEvent(new CustomEvent<T>(type, {detail: detail}))
    }

    //
    // Handling listeners
    //
    //private listeners_ = new Map<any, IHandler>();
    listeners_ = new Map<any, IHandler>()

    addListener(type: string, listener: any) {
        const func = (evt: any) => listener(evt.detail)
        this.on(type, func)
        this.listeners_.set(listener, {type: type, func: func})
    }

    removeListener(type: string, listener: any) {
        let handler = this.listeners_.get(listener)
        if (!handler) {
            //console.error("Listener  is missing: ", listener);
            return
        }
        this.remove(type, handler.func)
    }
}
