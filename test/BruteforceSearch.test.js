const { BruteforceSearch } = require('../index');

describe('BruteforceSearch', () => {
  describe('#constructor', () => {
    it('throws an error if no arguments are given', () => {
      expect(() => { new BruteforceSearch() }).toThrowError('wrong number of arguments (given 0, expected 2)');
    });

    it('throws an error if given a non-String object to first argument', () => {
      expect(() => { new BruteforceSearch(1, 3) }).toThrowError('wrong first argument type, expected String');
    });

    it('throws an error if given a non-Number object to second argument', () => {
      expect(() => { new BruteforceSearch('l2', '3') }).toThrowError('wrong second argument type, expected Number');
    });

    it('throws an error if given a String that is neither "l2" nor "ip" to first argument', () => {
      expect(() => { new BruteforceSearch('cos', 3) }).toThrowError('wrong space name, expected "l2" or "ip"');
    });
  });

  describe('#initIndex', () => {
    const index = new BruteforceSearch('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.initIndex() }).toThrowError('wrong number of arguments (given 0, expected 1)');
    });

    it('throws an error if given a non-Number argument', () => {
      expect(() => { index.initIndex('5') }).toThrowError('wrong argument type, expected Number');
    });
  });

  describe('#maxElements', () => {
    const index = new BruteforceSearch('l2', 3);

    it('returns 0 if called before the index is initialized', () => {
      expect(index.maxElements()).toBe(0);
    });

    it('returns maximum number of elements', () => {
      index.initIndex(10);
      expect(index.maxElements()).toBe(10);
    });
  });

  describe('#currentCount', () => {
    const index = new BruteforceSearch('l2', 3);

    it('returns 0 if called before the index is initialized', () => {
      expect(index.currentCount()).toBe(0);
    });

    it('returns current number of elements', () => {
      index.initIndex(5);
      index.addPoint([1, 2, 3], 0);
      index.addPoint([2, 3, 4], 1);
      expect(index.currentCount()).toBe(2);
    });
  });

  describe('#addPoint', () => {
    const index = new BruteforceSearch('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.addPoint() }).toThrowError('wrong number of arguments (given 0, expected 2)');
    });

    it('throws an error if given a non-Array object to first argument', () => {
      expect(() => { index.addPoint('[1, 2, 3]', 0) }).toThrowError('wrong first argument type, expected Array');
    });

    it('throws an error if given a non-Number object to second argument', () => {
      expect(() => { index.addPoint([1, 2, 3], '0') }).toThrowError('wrong second argument type, expected Number');
    });

    it('throws an error if called before the index is initialized', () => {
      expect(() => { index.addPoint([1, 2, 3], 0) }).toThrowError('search index has not been initialized, call `initIndex` in advance');
    });

    it('throws an error if given an array with a length different from the number of dimensions', () => {
      index.initIndex(1);
      expect(() => { index.addPoint([1, 2, 3, 4, 5], 0) }).toThrowError('invalid array length (given 5, expected 3)');
    });

    it('throws an error if more element is added than the maximum number of elements.', () => {
      index.initIndex(1);
      index.addPoint([1, 2, 3], 0);
      expect(() => { index.addPoint([1, 2, 3], 1) }).toThrowError(/Hnswlib Error/);
    });
  });

  describe('#removePoint', () => {
    const index = new BruteforceSearch('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.removePoint() }).toThrowError('wrong number of arguments (given 0, expected 1)');
    });

    it('throws an error if given a non-Number argument', () => {
      expect(() => { index.removePoint('0') }).toThrowError('wrong argument type, expected Number');
    });

    it('throws an error if called before the index is initialized', () => {
      expect(() => { index.removePoint(0) }).toThrowError('search index has not been initialized, call `initIndex` in advance');
    });

    it('removes the element specified by index', () => {
      index.initIndex(2);
      index.addPoint([1, 2, 3], 0);
      index.addPoint([1, 2, 4], 1);
      expect(index.currentCount()).toBe(2);
      index.removePoint(1);
      expect(index.currentCount()).toBe(1);
      expect(index.searchKnn([1, 2, 4], 1).neighbors).toEqual([0]);
    });
  });

  describe('#searchKnn', () => {
    describe('when metric space is "l2"', () => {
      const index = new BruteforceSearch('l2', 3);

      beforeAll(() => {
        index.initIndex(3);
        index.addPoint([1, 2, 3], 0);
        index.addPoint([2, 3, 4], 1);
        index.addPoint([3, 4, 5], 2);
      });

      it('throws an error if no arguments are given', () => {
        expect(() => { index.searchKnn() }).toThrowError('wrong number of arguments (given 0, expected 2)');
      });

      it('throws an error if given a non-Array object to first argument', () => {
        expect(() => { index.searchKnn('[1, 2, 3]', 2) }).toThrowError('wrong first argument type, expected Array');
      });

      it('throws an error if given a non-Number object to second argument', () => {
        expect(() => { index.searchKnn([1, 2, 3], '2') }).toThrowError('wrong second argument type, expected Number');
      });

      it('throws an error if given the number of neighborhoods exceeding the maximum number of elements', () => {
        expect(() => { index.searchKnn([1, 2, 5], 4) }).toThrowError('invalid number of k-nearest neighbors (cannot be given a value greater than `maxElements`: 3)');
      });

      it('throws an error if given an array with a length different from the number of dimensions', () => {
        expect(() => { index.searchKnn([1, 2, 5, 4], 2) }).toThrowError('invalid array length (given 4, expected 3)');
      });

      it('returns search results based on squared Euclidean distance', () => {
        expect(index.searchKnn([1, 2, 5], 2)).toMatchObject({ distances: [ 3, 4 ], neighbors: [ 1, 0 ] });
      });
    });

    describe('when metric space is "ip"', () => {
      const index = new BruteforceSearch('ip', 3);

      beforeAll(() => {
        index.initIndex(3);
        index.addPoint([1, 2, 3], 0);
        index.addPoint([2, 3, 4], 1);
        index.addPoint([3, 4, 5], 2);
      });

      it('returns search results based on one minus inner product', () => {
        expect(index.searchKnn([1, 2, 5], 2)).toMatchObject({ distances: [ -35, -27 ], neighbors: [ 2, 1 ] });
      });
    });
  });
});
