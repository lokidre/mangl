// @ts-check


export var content_ = '';

/** @param {string} [s] */
export function outclear(s) { 
    content_ = ''; 
    if (s) content_ += s;
}

/** @param {string} s */
export function outn(s) { content_ += s + '\n'; }

/** @param {string} s */
export function outnn(s) { content_ += s + '\n\n'; }

/** @param {number=} count */
export function nl(count) {
    let c = count ?? 1;
    for (let i = 0; i < c; ++i)
        content_ += '\n';
}
