
#ifndef _H_SST_CORE_STATS_HISTO
#define _H_SST_CORE_STATS_HISTO

#include <stdint.h>
#include <map>

using namespace std;

template<class HistoType>
class Histogram {
	public:
		Histogram(HistoType binW) {
			binCount = 0;
			binWidth = binW;
			bins = NULL;
		}

		void add(HistoType value) {
			if(NULL == bins) {
				bins = (HistoType*) malloc(sizeof(HistoType));
				bins[0] = 1;
				binCount = 1;

				minVal = (value - (value % binWidth));
				maxVal = minVal + binWidth;
			} else {
				if(value < minVal) {
					HistoType newLower = value - (value % binWidth);
					HistoType diff = minVal - newLower;
					int diff_bins = diff / binWidth;

					binCount += (uint32_t) diff_bins;
					HistoType* newBins = (HistoType*) malloc(binCount * sizeof(HistoType));

					for(uint32_t i = 0; i < (uint32_t) diff_bins; ++i) {
						newBins[i] = 0;
					}

					for(uint32_t i = (uint32_t) diff_bins; i < binCount; ++i) {
						newBins[i] = bins[i - diff_bins];
					}

					free(bins);
					bins = newBins;
					minVal = newLower;
				} else if (value >= maxVal) {
					HistoType newMax = value + (binWidth - (value % binWidth));
					HistoType diff = newMax - maxVal;
					int diff_bins = diff / binWidth;

					binCount += (uint32_t) diff_bins;
					HistoType* newBins = (HistoType*) malloc(binCount * sizeof(HistoType));

					for(uint32_t i = 0; i < (uint32_t) (binCount - diff_bins); ++i) {
						newBins[i] = bins[i];
					}

					for(uint32_t i = diff_bins; i < binCount; ++i) {
						newBins[i] = 0;
					}

					free(bins);
					bins = newBins;
					maxVal = newMax;
				}

				HistoType value_from_base = value - minVal;
				int inc_element = value_from_base / binWidth;

				bins[inc_element]++;
			}
		}

		uint32_t getBinCount() {
			return binCount;
		}

		HistoType getBinWidth() {
			return binWidth;
		}

		HistoType* getBinByIndex(int index) {
			return &bins[index];
		}

		HistoType getBinStart() {
			return minVal;
		}

		HistoType getBinMax() {
			return maxVal;
		}

	private:
		HistoType* bins;
		HistoType binWidth;
		HistoType minVal;
		HistoType maxVal;
		uint32_t binCount;
};

#endif
