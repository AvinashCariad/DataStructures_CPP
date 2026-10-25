/* 
	Given an array of intervals {start_time, end_time}, each interval is disjoint with the other. Given an input interval,
	return the update disjoint set of intervals. eg: <1,4>, <5,6>, <7,8> and input:<2,6>: <1,6>, <7,8>
	The array is sorted acc to starting time of each interval

	Input: array of intervals and interval
	Output: array of interval

	Solution:
		First add intervals before the input interval, once we get an intersecting interval, then take the union.
		Continue doing that. If we get an interval not intersecting add it.

		TC: O(n)
	*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Interval {
    int start;
    int end;
};

// Checks if two intervals intersect
bool isIntersecting(const Interval& a, const Interval& b) {
    return (a.end <= b.end && a.end >= b.start) ||
           (b.end <= a.end && b.end >= a.start);
}

// Merges a new interval into an UNSORTED set of intervals
vector<Interval> mergeDisjointIntervalsUnsorted(vector<Interval> intervals, Interval new_interval) {
    // 1. Sort the intervals based on their start times using a lambda function
    // TC: O(n log n)
    // sort using lamda function if not sorted
	sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
        return a.start < b.start;
    });

    vector<Interval> disjoint_intervals;
    int i = 0;
    int n = intervals.size();

    // 2. Add all non-intersecting intervals that sit BEFORE the new_interval
    while (i < n && !isIntersecting(intervals[i], new_interval) && intervals[i].end < new_interval.start) {
        disjoint_intervals.push_back(intervals[i]);
        ++i;
    }

    // 3. Consume and unify all overlapping intervals into new_interval
    while (i < n && isIntersecting(intervals[i], new_interval)) {
        new_interval.start = min(intervals[i].start, new_interval.start);
        new_interval.end = max(intervals[i].end, new_interval.end);
        ++i;
    }
    
    // Add the newly updated combined interval
    disjoint_intervals.push_back(new_interval);
    
    // 4. Add all remaining non-intersecting intervals (guaranteed to be AFTER)
    while (i < n) {
        disjoint_intervals.push_back(intervals[i]);
        ++i;
    }
        
    return disjoint_intervals;
}  

// Stream operator definition for printing
ostream& operator<<(ostream& out, const vector<Interval>& intervals) {
    for (const auto& interval : intervals) {
        out << "<" << interval.start << ", " << interval.end << ">\n";
    }
    return out;
}


// main code harness:
int main(int argc, char** argv) {
    // Input is deliberately scrambled and unsorted
    vector<Interval> intervals = {{14, 15}, {1, 5}, {-22, -1}, {11, 13}, {6, 10}};
    Interval new_interval = {2, 13};
    
    cout << "Merged Intervals (from unsorted input):\n";
    cout << mergeDisjointIntervalsUnsorted(intervals, new_interval);
    
    return 0;
}



/*

//Time Complexity: \(O(n \log n)\). Each of the n insertions into 
//the heap takes \(O(\log n)\) time. Popping the elements sequentially
//also takes \(O(n \log n)\). The merging logic itself inside the while
//loop is a swift O(1) per element.Space Complexity: O(n). The heap
//requires memory proportional to the number of intervals stored inside it. 


#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Interval {
    int start;
    int end;
};

// Custom comparator for the Min-Heap to sort intervals by start time
// Note that the logic can be more complex
struct CompareInterval {
    bool operator()(const Interval& a, const Interval& b) {
        // Greater-than builds a min-heap (smallest start time at the top)
        if (a.start != b.start) {
            return a.start > b.start;
        }
        return a.end > b.end; // Tie-breaker: earlier end time first
    }
};

// Merges unsorted intervals using a Min-Heap
std::vector<Interval> mergeIntervalsWithHeap(const std::vector<Interval>& intervals, Interval new_interval) {
    // 1. Initialize the Min-Heap
    std::priority_queue<Interval, std::vector<Interval>, CompareInterval> minHeap;

    // 2. Push all intervals plus the new one into the heap
    // Inserting each element takes O(log n)
    for (const auto& interval : intervals) {
        minHeap.push(interval);
    }
    minHeap.push(new_interval);

    std::vector<Interval> merged_intervals;
    if (minHeap.empty()) return merged_intervals;

    // 3. Pop the absolute earliest interval to initialize our tracking state
    Interval current = minHeap.top();
    minHeap.pop();

    // 4. Process the remaining intervals sequentially
    while (!minHeap.empty()) {
        Interval next = minHeap.top();
        minHeap.pop();

        // If 'next' overlaps with 'current', merge them
        if (next.start <= current.end) {
            current.end = std::max(current.end, next.end);
        } else {
            // No overlap: 'current' is fully complete, finalize it
            merged_intervals.push_back(current);
            current = next; // Move our tracking window forward
        }
    }

    // Don't forget to push the final remaining interval
    merged_intervals.push_back(current);

    return merged_intervals;
}

// Stream operator definition for clean printing
std::ostream& operator<<(std::ostream& out, const std::vector<Interval>& intervals) {
    for (const auto& interval : intervals) {
        out << "<" << interval.start << ", " << interval.end << ">\n";
    }
    return out;
}


// main code harness:
int main(int argc, char** argv) {
    // Input is completely unsorted
    std::vector<Interval> intervals = {{14, 15}, {1, 5}, {-22, -1}, {11, 13}, {6, 10}};
    Interval new_interval = {2, 13};
    
    std::cout << "Merged Intervals (using Min-Heap):\n";
    std::cout << mergeIntervalsWithHeap(intervals, new_interval);
    
    return 0;
}


*/



