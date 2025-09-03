module.exports = {
  extends: ['@commitlint/config-conventional'],
  ignores: [(message) => /^chore\((deps|deps-dev)\):\s+bump\s+/m.test(message)]
}
