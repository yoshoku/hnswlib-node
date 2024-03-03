import { expectType } from 'tsd';
import { BruteforceSearch, FilterFunction, HierarchicalNSW, InnerProductSpace, L2Space, SearchResult } from '.';

const l2Space = new L2Space(2);
expectType<L2Space>(l2Space);
expectType<number>(l2Space.getNumDimensions());
expectType<number>(l2Space.distance([1, 2], [3, 4]));
expectType<number>(l2Space.distance([1.5, 2.5], [3.5, 4.5]));

const ipSpace = new InnerProductSpace(2);
expectType<InnerProductSpace>(ipSpace);
expectType<number>(ipSpace.getNumDimensions());
expectType<number>(ipSpace.distance([1, 2], [3, 4]));
expectType<number>(ipSpace.distance([1.5, 2.5], [3.5, 4.5]));

const filter: FilterFunction = (label) => label % 2 === 0;

const bfSearch = new BruteforceSearch('l2', 2);
expectType<BruteforceSearch>(bfSearch);
expectType<void>(bfSearch.initIndex(2));
expectType<boolean>(await bfSearch.readIndex('tmp'));
expectType<void>(bfSearch.readIndexSync('tmp'));
expectType<boolean>(await bfSearch.writeIndex('tmp'));
expectType<void>(bfSearch.writeIndexSync('tmp'));
expectType<void>(bfSearch.addPoint([1, 2], 0));
expectType<void>(bfSearch.addPoint([3, 4], 1));
expectType<void>(bfSearch.removePoint(0));
expectType<SearchResult>(bfSearch.searchKnn([1, 1], 1));
expectType<SearchResult>(bfSearch.searchKnn([1, 1], 1, filter));
expectType<number>(bfSearch.getMaxElements());
expectType<number>(bfSearch.getCurrentCount());
expectType<number>(bfSearch.getNumDimensions());

const hnsw = new HierarchicalNSW('l2', 2);
expectType<HierarchicalNSW>(hnsw);
expectType<void>(hnsw.initIndex(2));
expectType<void>(hnsw.initIndex({ maxElements: 2 }));
expectType<boolean>(await hnsw.readIndex('tmp'));
expectType<boolean>(await hnsw.readIndex('tmp', true));
expectType<void>(await hnsw.readIndexSync('tmp'));
expectType<void>(await hnsw.readIndexSync('tmp', true));
expectType<boolean>(await hnsw.writeIndex('tmp'));
expectType<void>(await hnsw.writeIndexSync('tmp'));
expectType<void>(await hnsw.resizeIndex(5));
expectType<void>(hnsw.addPoint([1, 2], 0));
expectType<void>(hnsw.addPoint([3, 4], 1, true));
expectType<void>(hnsw.markDelete(1));
expectType<void>(hnsw.unmarkDelete(1));
expectType<SearchResult>(hnsw.searchKnn([1, 1], 1));
expectType<SearchResult>(hnsw.searchKnn([1, 1], 1, filter));
expectType<number[]>(hnsw.getIdsList());
expectType<number[]>(hnsw.getPoint(0));
expectType<number>(hnsw.getMaxElements());
expectType<number>(hnsw.getCurrentCount());
expectType<number>(hnsw.getNumDimensions());
expectType<number>(hnsw.getEf());
expectType<void>(hnsw.setEf(5));
