import js from '@eslint/js'
import vitest from '@vitest/eslint-plugin'
import neostandard from 'neostandard'

export default [
  {
    ignores: ['node_modules/**', 'lib/**', 'build/**', 'doc/**']
  },

  js.configs.recommended,
  ...neostandard(),

  {
    files: ['test/**/*.js', 'commitlint.config.js'],
    plugins: {
      vitest
    },
    languageOptions: {
      ecmaVersion: 'latest',
      sourceType: 'commonjs',
      globals: {
        ...vitest.environments.env.globals
      }
    },
    rules: {
      ...vitest.configs.recommended.rules
    }
  }
]
