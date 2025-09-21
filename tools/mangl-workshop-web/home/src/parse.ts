export default function parseBool(value: string | boolean | undefined, defaultValue = false) {
    if (value === undefined) return defaultValue
    if (typeof value === 'boolean') return value

    return (['true', 'false', true, false].includes(value) && JSON.parse(value)) || defaultValue
}