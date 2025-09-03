import js from '@eslint/js';
import jest from 'eslint-plugin-jest';
import jestExtended from 'eslint-plugin-jest-extended';
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
      jest,
      'jest-extended': jestExtended
    },
    languageOptions: {
      ecmaVersion: 'latest',
      sourceType: 'commonjs',
      globals: {
        ...jest.environments.globals.globals
      }
    },
    rules: {
      ...jest.configs.recommended.rules,
      ...jestExtended.configs.all.rules
    }
  }
];
