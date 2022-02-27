const { L2Space } = require('../index');

describe('L2Space', () => {
  it('throws an error if no arguments are given', () => {
    expect(() => { new L2Space() }).toThrowError('wrong number of arguments (given 0, expected 1)');
  });

  it('throws an error if given a non-Number argument', () => {
    expect(() => { new L2Space('yes') }).toThrowError(/wrong argument type, expected Number/);
  });

  const space = new L2Space(3);

  describe('#dim', () => {
    it('returns number of dimensions', () => {
      expect(space.dim()).toBe(3);
    });
  });

  describe('#distance', () => {
    it('throws an error if no arguments are given', () => {
      expect(() => { space.distance() }).toThrowError('wrong number of arguments (given 0, expected 2)');
    });

    it('throws an error if given a non-Array argument', () => {
      expect(() => { space.distance('foo', [0, 1, 2]) }).toThrowError(/wrong first argument type/);
      expect(() => { space.distance([0, 1, 2], 'bar') }).toThrowError(/wrong second argument type/);
    });

    it('throws an error if given an array with a length different from the number of dimensions', () => {
      expect(() => { space.distance([0, 1, 2, 3], [3, 4, 5]) }).toThrowError('wrong length of first array (given 4, expected 3)');
      expect(() => { space.distance([0, 1, 2], [3, 4, 5, 6]) }).toThrowError('wrong length of second array (given 4, expected 3)');
    });

    it('calculates squared Euclidean distance between two arrays', () => {
      expect(space.distance([1, 2, 3], [3, 4, 5])).toBeCloseTo(12.0, 8);
      expect(space.distance([0.1, 0.2, 0.3], [0.3, 0.4, 0.5])).toBeCloseTo(0.12, 8);
    });
  });
});
