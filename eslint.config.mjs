import js from '@eslint/js'
import neostandard from 'neostandard'

export default [
  {
    ignores: ['node_modules/**', 'lib/**', 'build/**', 'doc/**']
  },

  js.configs.recommended,
  ...neostandard(),

  {
    files: ['test/**/*.js', 'commitlint.config.js'],
    languageOptions: {
      ecmaVersion: 'latest',
      sourceType: 'commonjs',
    }
  }
]
