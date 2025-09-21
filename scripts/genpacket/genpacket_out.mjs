// @ts-check

export var source_ = '';
export var indentLevel_ = 0;
export var indentSpaces_ = '';
let indentBlock_ = '    ';

export function clearSource() {
    source_ = '';
    indentLevel_ = 0;
    updateIndent()
}

export function getSource() {
    return source_;
}

export function appendSource(text) {
    source_ += text;
}


export function out(s, indent, newlines) {
    source_ += indentSpaces_;

    // Additional indent
    if (indent !== undefined && indent > 0) {
        for (let i = 0; i < indent; ++i)
            source_ += indentBlock_;
    }

    source_ += s;
    source_ += '\n';

    if (newlines !== undefined && newlines > 0) {
        for (let i = 0; i < newlines; ++i)
            source_ += '\n';
    }
}

/** 
 * @param {string} s 
 * */
export function outn(s) {
    out(s);
    nl();
}

export function out_ind(s) {
    out(s, 1);
}

export function nl(count) {
    if (count === undefined)
        count = 1;
    for (let i = 0; i < count; ++i)
        source_ += '\n';
}



export function indent(count) {
    if (count === undefined)
        count = 1;
    indentLevel_ += count;
    updateIndent();
}

export function unindent(count) {
    if (count === undefined)
        count = 1;

    indentLevel_ -= count;
    if (indentLevel_ < 0)
        indentLevel_ = 0;
    updateIndent();
}


function updateIndent() {
    indentSpaces_ = '';
    for (let i = 0; i < indentLevel_; ++i) {
        indentSpaces_ += indentBlock_;
    }
}

export function beginBlock(statement) {
    let stmt = statement ? `${statement} ` : '';
    stmt += '{';
    out(stmt);
    indent();

}

export function endBlock(comment) {
    unindent();

    let closing = comment ? `}  // ${comment}` : '}'
    out(closing);
}
