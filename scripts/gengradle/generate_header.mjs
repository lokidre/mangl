/**
 * @param {import ("./gengradle.d.ts").Lang} lang
 * @returns {string}
 **/

export function generateHeader(lang) {
    let genDate = new Date();
    let dateString = genDate.toISOString().split('T')[0];

    let content = '';

    let commentPrefix, commentSuffix;

    switch (lang) {
        case 'cmake':
            commentPrefix = commentSuffix = '#';
            break;
        case 'gradle':
        case 'cpp':
        case 'java':
        case 'kotlin':
            commentPrefix = commentSuffix = '//';
            break;
        case 'xml':
            content = `<?xml version="1.0" encoding="utf-8"?>\n`;
            commentPrefix = "<!--";
            commentSuffix = "-->";
            break;
    }

    content += `${commentPrefix} *** Automatically Generated *** DO NOT EDIT *** ${commentSuffix}\n`;
    content += `${commentPrefix} *** Generated Date: ${dateString} *** ${commentSuffix}\n`;
    content += '\n';

    return content;
}
