## [[1.1.0](https://github.com/yoshoku/hnswlib-node/compare/v1.0.3...v1.1.0)] - 2022-04-29

- Add `readIndex`, `writeIndex`, `readIndexSync`, and `writeIndexSync` functions to BruteforeceSearch and HierarchicalNSW.
- Deprecate `loadIndex` and `saveIndex` functions on BruteforeceSearch and HierarchicalNSW,
use `readIndexSync` and `writeIndexSync` instead.

## [[1.0.3](https://github.com/yoshoku/hnswlib-node/compare/v1.0.2...v1.0.3)] - 2022-04-14

- Change array type notation in declaration file.

## [[1.0.2](https://github.com/yoshoku/hnswlib-node/compare/v1.0.1...v1.0.2)] - 2022-03-21

- Change to call the constructor when loading the search index on the BruteforeceSearch class.
- Fix to use the member variable of the original BruteforeceSearch class for the maximum number of elements.

## [[1.0.1](https://github.com/yoshoku/hnswlib-node/compare/v1.0.0...v1.0.1)] - 2022-03-19

- Add API documentation.

## [1.0.0] - 2022-03-13

- Initial release.
