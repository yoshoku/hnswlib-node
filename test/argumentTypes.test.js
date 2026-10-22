const { BruteforceSearch, HierarchicalNSW, InnerProductSpace, L2Space } = require('../lib')

const withThrowingElement = (arr, index) => {
  const copy = [...arr]
  Object.defineProperty(copy, index, {
    get() {
      throw new Error('boom')
    }
  })
  return copy
}

const throwingFilter = () => {
  throw new Error('boom')
}

describe('L2Space#distance', () => {
  const space = new L2Space(3)

  it('throws an error if given an array containing a non-Number element', () => {
    expect(() => {
      space.distance([1, 2, 'c'], [1, 2, 3])
    }).toThrow('Invalid the first array type, must be an Array of numbers.')
    expect(() => {
      space.distance([1, 2, 3], [1, 2, 'c'])
    }).toThrow('Invalid the second array type, must be an Array of numbers.')
  })

  it('propagates an error raised by an element getter', () => {
    expect(() => {
      space.distance(withThrowingElement([1, 2, 3], 0), [1, 2, 3])
    }).toThrow('boom')
  })
})

describe('InnerProductSpace#distance', () => {
  const space = new InnerProductSpace(3)

  it('throws an error if given an array containing a non-Number element', () => {
    expect(() => {
      space.distance([1, 2, 'c'], [1, 2, 3])
    }).toThrow('Invalid the first array type, must be an Array of numbers.')
    expect(() => {
      space.distance([1, 2, 3], [1, 2, 'c'])
    }).toThrow('Invalid the second array type, must be an Array of numbers.')
  })

  it('propagates an error raised by an element getter', () => {
    expect(() => {
      space.distance([1, 2, 3], withThrowingElement([1, 2, 3], 2))
    }).toThrow('boom')
  })
})

describe('BruteforceSearch#addPoint', () => {
  const index = new BruteforceSearch('l2', 3)

  beforeAll(() => {
    index.initIndex(2)
  })

  it('does not add a point given an array containing a non-Number element', () => {
    expect(() => {
      index.addPoint([1, 2, 'c'], 0)
    }).toThrow('Invalid the given array type, must be an Array of numbers.')
    expect(index.getCurrentCount()).toBe(0)
  })
})

describe('BruteforceSearch#searchKnn', () => {
  const index = new BruteforceSearch('l2', 3)

  beforeAll(() => {
    index.initIndex(2)
    index.addPoint([1, 2, 3], 0)
  })

  it('throws an error if given an array containing a non-Number element', () => {
    expect(() => {
      index.searchKnn([1, 2, 'c'], 1)
    }).toThrow('Invalid the given array type, must be an Array of numbers.')
  })

  it('propagates an error raised by the filter function', () => {
    expect(() => {
      index.searchKnn([1, 2, 3], 1, throwingFilter)
    }).toThrow('boom')
  })
})

describe('HierarchicalNSW#addPoint', () => {
  const index = new HierarchicalNSW('l2', 3)

  beforeAll(() => {
    index.initIndex(2)
    index.addPoint([1, 2, 3], 0)
  })

  it('does not add a point given an array containing a non-Number element', () => {
    expect(() => {
      index.addPoint([1, 2, 'c'], 1)
    }).toThrow('Invalid the given array type, must be an Array of numbers.')
    expect(index.getCurrentCount()).toBe(1)
  })
})

describe('HierarchicalNSW#searchKnn', () => {
  const index = new HierarchicalNSW('l2', 3)

  beforeAll(() => {
    index.initIndex(2)
    index.addPoint([1, 2, 3], 0)
  })

  it('throws an error if given an array containing a non-Number element', () => {
    expect(() => {
      index.searchKnn([1, 2, 'c'], 1)
    }).toThrow('Invalid the given array type, must be an Array of numbers.')
  })

  it('propagates an error raised by the filter function', () => {
    expect(() => {
      index.searchKnn([1, 2, 3], 1, throwingFilter)
    }).toThrow('boom')
  })
})

describe('HierarchicalNSW#initIndex', () => {
  it('reads each named argument once', () => {
    const index = new HierarchicalNSW('l2', 3)
    let reads = 0
    index.initIndex({
      maxElements: 4,
      get m() {
        reads++
        return 16
      }
    })
    expect(reads).toBe(1)
    expect(index.getMaxElements()).toBe(4)
  })

  it('builds a usable index if a named argument getter returns a different value on each read', () => {
    const index = new HierarchicalNSW('l2', 3)
    let reads = 0
    index.initIndex({
      maxElements: 4,
      get m() {
        return reads++ === 0 ? 16 : 'x'
      }
    })
    expect(index.getMaxElements()).toBe(4)
    index.addPoint([1, 2, 3], 0)
    index.addPoint([2, 3, 4], 1)
    expect(index.getCurrentCount()).toBe(2)
  })

  it('keeps the current index if a named argument getter raises', () => {
    const index = new HierarchicalNSW('l2', 3)
    index.initIndex(2)
    index.addPoint([1, 2, 3], 0)
    expect(() => {
      index.initIndex({
        maxElements: 4,
        get m() {
          throw new Error('boom')
        }
      })
    }).toThrow('boom')
    expect(index.getMaxElements()).toBe(2)
    expect(index.getCurrentCount()).toBe(1)
  })
})
