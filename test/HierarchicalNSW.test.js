const { HierarchicalNSW } = require('../lib');

describe('HierarchicalNSW', () => {
  describe('#constructor', () => {
    it('throws an error if no arguments are given', () => {
      expect(() => { new HierarchicalNSW() }).toThrowError('Expected 2 arguments, but got 0.');
    });

    it('throws an error if given a non-String object to first argument', () => {
      expect(() => { new HierarchicalNSW(1, 3) }).toThrowError('Invalid the first argument type, must be a string.');
    });

    it('throws an error if given a non-Number object to second argument', () => {
      expect(() => { new HierarchicalNSW('l2', '3') }).toThrowError('Invalid the second argument type, must be a number.');
    });

    it('throws an error if given a String that is neither "l2" nor "ip" to first argument', () => {
      expect(() => { new HierarchicalNSW('cos', 3) }).toThrowError('Wrong space name, expected "l2" or "ip".');
    });
  });

  describe('#initIndex', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.initIndex() }).toThrowError('Expected 1-4 arguments, but got 0.');
    });

    it('throws an error if given a non-Number argument', () => {
      expect(() => { index.initIndex('5') }).toThrowError('Invalid the first argument type, must be a number.');
    });
  });

  describe('#resizeIndex', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.resizeIndex() }).toThrowError('Expected 1 arguments, but got 0.');
    });

    it('throws an error if given a non-Number argument', () => {
      expect(() => { index.resizeIndex('0') }).toThrowError('Invalid the first argument type, must be a number.');
    });

    it('throws an error if called before the index is initialized', () => {
      expect(() => { index.resizeIndex(5) }).toThrowError('Search index has not been initialized, call `initIndex` in advance.');
    });

    it('marks the element as deleted', () => {
      index.initIndex(2);
      index.addPoint([1, 2, 3], 0);
      index.addPoint([2, 3, 4], 1);
      expect(() => { index.addPoint([3, 4, 5], 2) }).toThrowError(/Hnswlib Error/);
      index.resizeIndex(3);
      index.addPoint([3, 4, 5], 2);
      expect(index.getMaxElements()).toBe(3);
    });
  });

  describe('#getIdsList', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('returns an empty array if called before the index is initialized', () => {
      expect(index.getIdsList()).toMatchObject([]);
    });

    it('returns an array consists of label id', () => {
      index.initIndex(5);
      index.addPoint([1, 2, 3], 0);
      index.addPoint([2, 3, 4], 1);
      expect(index.getIdsList()).toMatchObject([1, 0]);
    });
  });

  describe('#getMaxElements', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('returns 0 if called before the index is initialized', () => {
      expect(index.getMaxElements()).toBe(0);
    });

    it('returns maximum number of elements', () => {
      index.initIndex(10);
      expect(index.getMaxElements()).toBe(10);
    });
  });

  describe('#getCurrentCount', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('returns 0 if called before the index is initialized', () => {
      expect(index.getCurrentCount()).toBe(0);
    });

    it('returns current number of elements', () => {
      index.initIndex(5);
      index.addPoint([1, 2, 3], 0);
      index.addPoint([2, 3, 4], 1);
      expect(index.getCurrentCount()).toBe(2);
    });
  });

  describe('#getNumDimensions', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('returns number of dimensions', () => {
      expect(index.getNumDimensions()).toBe(3);
    });
  });

  describe('#getEf', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('throws an error if called before the index is initialized', () => {
      expect(() => { index.getEf() }).toThrowError('Search index has not been initialized, call `initIndex` in advance.');
    });

    it('returns ef parameter value', () => {
      index.initIndex(3);
      expect(index.getEf()).toBe(10);
    });
  });

  describe('#setEf', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.setEf() }).toThrowError('Expected 1 arguments, but got 0.');
    });

    it('throws an error if given a non-Number argument', () => {
      expect(() => { index.setEf('0') }).toThrowError('Invalid the first argument type, must be a number.');
    });


    it('throws an error if called before the index is initialized', () => {
      expect(() => { index.setEf(123) }).toThrowError('Search index has not been initialized, call `initIndex` in advance.');
    });

    it('sets ef parameter value', () => {
      index.initIndex(3);
      index.setEf(123);
      expect(index.getEf()).toBe(123);
    });
  });

  describe('#addPoint', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.addPoint() }).toThrowError('Expected 2 arguments, but got 0.');
    });

    it('throws an error if given a non-Array object to first argument', () => {
      expect(() => { index.addPoint('[1, 2, 3]', 0) }).toThrowError('Invalid the first argument type, must be an Array.');
    });

    it('throws an error if given a non-Number object to second argument', () => {
      expect(() => { index.addPoint([1, 2, 3], '0') }).toThrowError('Invalid the second argument type, must be a number.');
    });

    it('throws an error if called before the index is initialized', () => {
      expect(() => { index.addPoint([1, 2, 3], 0) }).toThrowError('Search index has not been initialized, call `initIndex` in advance.');
    });

    it('throws an error if given an array with a length different from the number of dimensions', () => {
      index.initIndex(1);
      expect(() => { index.addPoint([1, 2, 3, 4, 5], 0) }).toThrowError('Invalid the given array length (expected 3, but got 5).');
    });

    it('throws an error if more element is added than the maximum number of elements.', () => {
      index.initIndex(1);
      index.addPoint([1, 2, 3], 0);
      expect(() => { index.addPoint([1, 2, 3], 1) }).toThrowError(/Hnswlib Error/);
    });
  });

  describe('#markDelete', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.markDelete() }).toThrowError('Expected 1 arguments, but got 0.');
    });

    it('throws an error if given a non-Number argument', () => {
      expect(() => { index.markDelete('0') }).toThrowError('Invalid the first argument type, must be a number.');
    });

    it('throws an error if called before the index is initialized', () => {
      expect(() => { index.markDelete(0) }).toThrowError('Search index has not been initialized, call `initIndex` in advance.');
    });

    it('marks the element as deleted', () => {
      index.initIndex(2);
      index.addPoint([1, 2, 3], 0);
      index.addPoint([1, 2, 4], 1);
      index.markDelete(1);
      expect(index.searchKnn([1, 2, 4], 1).neighbors).toEqual([0]);
    });
  });

  describe('#unmarkDelete', () => {
    const index = new HierarchicalNSW('l2', 3);

    it('throws an error if no arguments are given', () => {
      expect(() => { index.unmarkDelete() }).toThrowError('Expected 1 arguments, but got 0.');
    });

    it('throws an error if given a non-Number argument', () => {
      expect(() => { index.unmarkDelete('0') }).toThrowError('Invalid the first argument type, must be a number.');
    });

    it('throws an error if called before the index is initialized', () => {
      expect(() => { index.unmarkDelete(0) }).toThrowError('Search index has not been initialized, call `initIndex` in advance.');
    });

    it('unmarks the element as deleted', () => {
      index.initIndex(2);
      index.addPoint([1, 2, 3], 0);
      index.addPoint([1, 2, 4], 1);
      index.markDelete(1);
      expect(index.searchKnn([1, 2, 4], 1).neighbors).toEqual([0]);
      index.unmarkDelete(1);
      expect(index.searchKnn([1, 2, 4], 1).neighbors).toEqual([1]);
    });
  });

  describe('#searchKnn', () => {
    describe('when metric space is "l2"', () => {
      const index = new HierarchicalNSW('l2', 3);

      beforeAll(() => {
        index.initIndex(3);
        index.addPoint([1, 2, 3], 0);
        index.addPoint([2, 3, 4], 1);
        index.addPoint([3, 4, 5], 2);
      });

      it('throws an error if no arguments are given', () => {
        expect(() => { index.searchKnn() }).toThrowError('Expected 2 arguments, but got 0.');
      });

      it('throws an error if given a non-Array object to first argument', () => {
        expect(() => { index.searchKnn('[1, 2, 3]', 2) }).toThrowError('Invalid the first argument type, must be an Array.');
      });

      it('throws an error if given a non-Number object to second argument', () => {
        expect(() => { index.searchKnn([1, 2, 3], '2') }).toThrowError('Invalid the second argument type, must be a number.');
      });

      it('throws an error if given the number of neighborhoods exceeding the maximum number of elements', () => {
        expect(() => { index.searchKnn([1, 2, 5], 4) }).toThrowError('Invalid the number of k-nearest neighbors (cannot be given a value greater than `maxElements`: 3).');
      });

      it('throws an error if given an array with a length different from the number of dimensions', () => {
        expect(() => { index.searchKnn([1, 2, 5, 4], 2) }).toThrowError('Invalid the given array length (expected 3, but got 4).');
      });

      it('returns search results based on squared Euclidean distance', () => {
        expect(index.searchKnn([1, 2, 5], 2)).toMatchObject({ distances: [ 3, 4 ], neighbors: [ 1, 0 ] });
      });
    });

    describe('when metric space is "ip"', () => {
      const index = new HierarchicalNSW('ip', 3);

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
