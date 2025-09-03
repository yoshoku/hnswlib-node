const { InnerProductSpace } = require('../lib')

describe('InnerProductSpace', () => {
  it('throws an error if no arguments are given', () => {
    expect(() => { new InnerProductSpace() }).toThrow('Expected 1 arguments, but got 0.')
  })

  it('throws an error if given a non-Number argument', () => {
    expect(() => { new InnerProductSpace('yes') }).toThrow('Invalid the first argument type, must be a number.')
  })

  const space = new InnerProductSpace(3)

  describe('#getNumDimensions', () => {
    it('returns number of dimensions', () => {
      expect(space.getNumDimensions()).toBe(3)
    })
  })

  describe('#distance', () => {
    it('throws an error if no arguments are given', () => {
      expect(() => { space.distance() }).toThrow('Expected 2 arguments, but got 0.')
    })

    it('throws an error if given a non-Array argument', () => {
      expect(() => { space.distance('foo', [0, 1, 2]) }).toThrow('Invalid the first argument type, must be an Array.')
      expect(() => { space.distance([0, 1, 2], 'bar') }).toThrow('Invalid the second argument type, must be an Array.')
    })

    it('throws an error if given an array with a length different from the number of dimensions', () => {
      expect(() => { space.distance([0, 1, 2, 3], [3, 4, 5]) }).toThrow('Invalid the first array length (expected 3, but got 4).')
      expect(() => { space.distance([0, 1, 2], [3, 4, 5, 6]) }).toThrow('Invalid the second array length (expected 3, but got 4).')
    })

    it('calculates one minus inner product between two arrays', () => {
      expect(space.distance([1, 2, 3], [3, 4, 5])).toBeCloseTo(-25.0, 6)
      expect(space.distance([0.1, 0.2, 0.3], [0.3, 0.4, 0.5])).toBeCloseTo(0.74, 6)
    })
  })
})
