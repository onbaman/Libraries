/* 
 	A median filter object is created by by passing the desired filter window size on object creation.
	The window size should be an odd number between 3 and 255.  
  
	New data is added to the median filter by passing the data through the in() function.  The new medial value is returned.
	The new data will over-write the oldest data point, then be shifted in the array to place it in the correct location. 
  
	The current median value is returned by the out() function for situations where the result is desired without passing in new data.
 
	!!! All data must be type INT.  !!!
	
	Window Size / avg processing time [us]
	5  / 22
	7  / 30
	9  / 40
	11 / 49
	21 / 99
	
*/

#include "IRFilter.h"

IRFilter::IRFilter(const byte size, const int seed, long a0, long a1, long a2, long a3, long a4, long a5)
{

	A0 = a0;
	A1 = a1;
	A2 = a2;
	A3 = a3;
	A4 = a4;
	A5 = a5;
	medFilterWin = max(size, 3);			// number of samples in sliding median filter window - usually odd #
	medDataPointer = size >> 1;				// mid point of window
	data = (int*) calloc (size, sizeof(int));			// array for data
	sizeMap = (byte*) calloc (size, sizeof(byte));		// array for locations of data in sorted list
	locationMap = (byte*) calloc (size, sizeof(byte));	// array for locations of history data in map list
	oldestDataPoint = medDataPointer;					// oldest data point location in data array
	
	for(byte i = 0; i < medFilterWin; i++) // initialize the arrays
	{
		sizeMap[i] = i;			// start map with straight run
		locationMap[i] = i;		// start map with straight run
		data[i] = seed;			// populate with seed value
	}
	
}

int IRFilter::in(int value)
{  
	// sort sizeMap
	// small vaues on the left (-)
	// larger values on the right (+)

	boolean dataMoved = false;
	const byte rightEdge = medFilterWin - 1;  // adjusted for zero indexed array
	
	data[oldestDataPoint] = value;  // store new data in location of oldest data in ring buffer
	
	// SORT LEFT (-) <======(n) (+)
	if(locationMap[oldestDataPoint] > 0){ // don't check left neighbours if at the extreme left
		
		for(byte i = locationMap[oldestDataPoint]; i > 0; i--){	//index through left adjacent data
			
			byte n = i - 1;	// neighbour location
			
			if(data[oldestDataPoint] < data[sizeMap[n]]){ // find insertion point, move old data into position

				sizeMap[i] = sizeMap[n];	// move existing data right so the new data can go left
				locationMap[sizeMap[n]]++;
				
				sizeMap[n] = oldestDataPoint; // assign new data to neighbor position
				locationMap[oldestDataPoint]--;
				
				dataMoved = true;
				
			}
			else {

				break; // stop checking once a smaller value is found on the left 
				
			}
		}
	}
	
	// SORT RIGHT (-) (n)======> (+)
	if(!dataMoved && locationMap[oldestDataPoint] < rightEdge){ // don't check right if at right border, or the data has already moved

		for(int i = locationMap[oldestDataPoint]; i < rightEdge; i++){	//index through left adjacent data

			int n = i + 1;	// neighbour location
			
			if(data[oldestDataPoint] > data[sizeMap[n]]){ // find insertion point, move old data into position

				sizeMap[i] = sizeMap[n];	// move existing data left so the new data can go right
				locationMap[sizeMap[n]]--;
				
				sizeMap[n] = oldestDataPoint; // assign new data to neighbor position
				locationMap[oldestDataPoint]++;

			}
			else {

				break; // stop checking once a smaller value is found on the right 
				
			}	
		}
	}	
	
	oldestDataPoint++; 		// increment and wrap
	if(oldestDataPoint == medFilterWin) 
		oldestDataPoint = 0;
	
	return data[sizeMap[medDataPointer]];
}


int IRFilter::poly(int i){
  return (int)(((((A5*i + A4)*i + A3)*i + A2)*i + A1)*i + A0);
}


int IRFilter::out() // return the value of the median data sample
{
	return  poly(data[sizeMap[medDataPointer]]);
}


