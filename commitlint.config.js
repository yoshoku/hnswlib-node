module.exports = {
  extends: ['@commitlint/config-conventional'],
  ignores: [(message) => /^chore\((deps|deps-dev)\): bump\ .+ from .+ to +./m.test(message)]
};
