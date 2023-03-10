## [[1.4.2](https://github.com/yoshoku/hnswlib-node/compare/v1.4.1...v1.4.2)] - 2023-03-10

- Add support for cosine space option to constructor of HierarchicalNSW and BruteforceSearch ([#51](https://github.com/yoshoku/hnswlib-node/issues/51)).
  ```javascript
  import { HierarchicalNSW } from 'hnswlib-node';

  // Setting the spaceName argument to 'cosine'.
  const index = new HierarchicalNSW('cosine', 3);
  index.initIndex({ maxElements: 3 });

  index.addPoint([1, 2, 3], 1);
  index.addPoint([4, 5, 6], 2);
  index.addPoint([7, 8, 9], 3);

  // Searching based on cosine distance (1 subtract cosine similarity).
  console.table(index.searchKnn([1, 2, 2], 2));
  // ┌───────────┬──────────────────────┬──────────────────────┐
  // │  (index)  │          0           │          1           │
  // ├───────────┼──────────────────────┼──────────────────────┤
  // │ distances │ 0.012341678142547607 │ 0.018790483474731445 │
  // │ neighbors │          2           │          3           │
  // └───────────┴──────────────────────┴──────────────────────┘
  ```
- Update dev-dependencies.

## [[1.4.1](https://github.com/yoshoku/hnswlib-node/compare/v1.4.0...v1.4.1)] - 2023-03-05

- Add `getPoint` function to HierarchicalNSW.
  ```javascript
  import { HierarchicalNSW } from 'hnswlib-node';

  const index = new HierarchicalNSW('l2', 3);
  index.initIndex({ maxElements: 2 });

  index.addPoint([1, 2, 3], 1);
  index.addPoint([4, 5, 6], 2);

  console.log(index.getPoint(1));
  // [ 1, 2, 3 ]
  console.log(index.getPoint(2));
  // [ 4, 5, 6 ]
  ```
- Fix example codes in CHANGELOG.
- Update dev-dependencies.

## [[1.4.0](https://github.com/yoshoku/hnswlib-node/compare/v1.3.0...v1.4.0)] - 2023-02-26

- Add development directories to npmignore.
- Update dependencies: node-addon-api@6.0.0.
- Update and add dev-dependencies.
- Fix some config files.

## [[1.3.0](https://github.com/yoshoku/hnswlib-node/compare/v1.2.0...v1.3.0)] - 2023-02-19

- Updete bundled hnswlib to v0.7.0.
- Add support for named arguments in `initIndex` of HierarchicalNSW.
- Add support for replacing an element marked for deletion with a new element.
  ```javascript
  import { HierarchicalNSW } from 'hnswlib-node';

  const index = new HierarchicalNSW('l2', 3);
  index.initIndex({ maxElements: 2 });

  index.addPoint([1, 2, 3], 1);
  index.addPoint([4, 5, 6], 2);

  index.markDelete(1);

  // HierarchicalNSW throws the error here since the number of data points has reached maxElements:
  // Hnswlib Error: The number of elements exceeds the specified limit
  index.addPoint([6, 5, 4], 3);
  ```
  ```javascript
  import { HierarchicalNSW } from 'hnswlib-node';

  const index = new HierarchicalNSW('l2', 3);

  // Initizaling the index with `allowReplaceDeleted` set to true.
  index.initIndex({ maxElements: 2, allowReplaceDeleted: true });

  index.addPoint([1, 2, 3], 1);
  index.addPoint([4, 5, 6], 2);

  index.markDelete(1);

  // By setting the third argument (replaceDeleted) to true, HierarchicalNSW does not throw the error
  // since the newly added datum point will replace the deleted datum point.
  index.addPoint([6, 5, 4], 3, true);
  ```
- Add support filtering function by label in `searchKnn` of BruteforeceSearch and HierarchicalNSW.
  ```javascript
  import { HierarchicalNSW } from 'hnswlib-node';

  const index = new HierarchicalNSW('l2', 3);
  index.initIndex({ maxElements: 4 });

  index.addPoint([1, 2, 3], 1);
  index.addPoint([1, 3, 3], 2);
  index.addPoint([1, 2, 3], 3);
  index.addPoint([1, 3, 3], 4);

  // setting filter funtion that allows only even labels.
  const filter = (label: number): boolean => label % 2 == 0;
  console.table(index.searchKnn([1, 2, 3], 2, filter));
  // ┌───────────┬───┬───┐
  // │  (index)  │ 0 │ 1 │
  // ├───────────┼───┼───┤
  // │ distances │ 1 │ 1 │
  // │ neighbors │ 2 │ 4 │
  // └───────────┴───┴───┘
  ```
- Remove deprecated functions `loadIndex` and `saveIndex` from BruteforeceSearch and HierarchicalNSW.
- Update dev-dependencies.

## [[1.2.0](https://github.com/yoshoku/hnswlib-node/compare/v1.1.0...v1.2.0)] - 2022-11-26

- Update dependencies: node-addon-api@5.0.0.
- Update dev-dependencies.

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
