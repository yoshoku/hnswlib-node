export type SpaceName = 'l2' | 'ip';

export interface SearchResult {
  distances: Array<number>,
  neighbors: Array<number>
}

export class L2Space {
  constructor(numDimensions: number);
  distance(pointA: Array<number>, pointB: Array<number>): number;
  getNumDimensions(): number;
}

export class InnerProductSpace {
  constructor(numDimensions: number);
  distance(pointA: Array<number>, pointB: Array<number>): number;
  getNumDimensions(): number;
}

export class BruteforceSearch {
  constructor(spaceName: SpaceName, numDimensions: number);
  initIndex(maxElements: number): void;
  loadIndex(filename: string): void;
  saveIndex(filename: string): void;
  addPoint(point: Array<number>, label: number): void;
  removePoint(label: number): void;
  searchKnn(queryPoint: Array<number>, numNeighbors: number): SearchResult;
  getMaxElements(): number;
  getCurrentCount(): number;
  getNumDimensions(): number;
}

export class HierarchicalNSW {
  constructor(spaceName: SpaceName, numDimensions: number);
  initIndex(maxElements: number, m?: number, efConstruction?: number, randomSeed?: number): void;
  loadIndex(filename: string): void;
  saveIndex(filename: string): void;
  resizeIndex(newMaxElements: number): void;
  addPoint(point: Array<number>, label: number): void;
  markDelete(label: number): void;
  unmarkDelete(label: number): void;
  searchKnn(queryPoint: Array<number>, numNeighbors: number): SearchResult;
  getIdsList(): Array<number>;
  getMaxElements(): number;
  getCurrentCount(): number;
  getNumDimensions(): number;
  getEf(): number;
  setEf(ef: number): void;
}
