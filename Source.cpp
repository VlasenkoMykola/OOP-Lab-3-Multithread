using namespace std;

#include <iostream>
#include <stdio.h>
#include <random>
#include <chrono>
#include <thread>
#include "unittest.h"
#include <cstring>

// -------------------------------------------------------------------

/**
euclid algorithm, for Greatest Common Divisor, returns the divisor
*/
int gcd(int numer, int denom) {

    while (numer != denom) {
        if (numer > denom) {
            numer = numer - denom;
        }
        else {
            denom = denom - numer;
        }
    }

    return numer;
}

/**
rational number class
*/
class rational
{
    int numer;
    int denom;
public:
    rational() {
        numer = 0;
        denom = 1;
    }
    rational(int numer_new) {
        numer = numer_new;
        denom = 1;
    }
    rational(int numer_new, int denom_new) {
        if (!numer_new) {
            numer = 0;
            denom = 1;
        }
        else {
            int sign = 1;
            if (numer_new < 0) { sign = -sign; numer_new = -numer_new; };
            if (denom_new < 0) { sign = -sign; denom_new = -denom_new; };

            int gsd_new = gcd(numer_new, denom_new);
            //cout << numer_new << ':' << denom_new << " gsd: " << gsd_new << endl;
            if (gsd_new) {
                numer = sign * numer_new / gsd_new;
                denom = denom_new / gsd_new;
            }
            else {
                numer = sign * numer_new;
                denom = denom_new;
            }
        }
    }
    rational(int numer_new, int denom_new, int nooptimize) {
        numer = numer_new;
        denom = denom_new;
    }
    bool is1() { return numer && numer == denom; };
    bool is0() { return !numer; };
    rational operator+(rational);
    rational operator-(rational);
    rational operator *(rational);
    rational operator /(rational);
    bool operator ==(rational);
    int get_numer() { return numer; };
    int get_denom() { return denom; };
};

/**
overloaded + operator
*/
rational rational ::operator+(rational c1)
{
    if (denom == 1 && c1.denom == 1) {
        return rational(numer + c1.numer, denom, 0);
    }
    else {
        return rational((numer * c1.denom) + (c1.numer * denom), denom * c1.denom);
    }
}

/**
overloaded - operator
*/
rational rational ::operator -(rational c1)
{
    if (denom == 1 && c1.denom == 1) {
        return rational(numer - c1.numer, denom, 0);
    }
    else {
        return rational((numer * c1.denom) - (c1.numer * denom), denom * c1.denom);
    }
}

/**
overloaded * operator
*/
rational rational ::operator *(rational c1)
{
    if (denom == 1 && c1.denom == 1) {
        return rational(numer * c1.numer, denom, 0);
    }
    else {
        return rational(numer * c1.numer, denom * c1.denom);
    }
}

/**
overloaded == operator
*/
bool rational ::operator ==(rational c1) {
    return (numer == c1.numer) && (denom == c1.denom);
}

/**
overloaded / operator
*/
rational rational :: operator /(rational c1)
{
    return rational(numer * c1.denom, c1.numer * denom);
}

static default_random_engine generator;
static uniform_int_distribution<int> distribution_numer(-100, 100);
static uniform_int_distribution<int> distribution_denom(1, 100);

/**
returns semi-random rational number, between -1000 and 1000
*/
rational rand_rational() {
    return rational(distribution_numer(generator), distribution_denom(generator));
}


/**
overloaded << operator
*/
std::ostream& operator<<(std::ostream& s, rational& r) {
    if (r.get_denom() == 1) {
        return s << r.get_numer();
    }
    else {
        //return s << "(" << r.get_numer() << "/" << r.get_denom() << ")";
        return s << r.get_numer() << "/" << r.get_denom();
    }
}

/**
function for testing rational numbers
*/
void test_rational() {
    unittest T = unittest("rational");
    T.ok(rational(1, 2) + rational(1, 2) == rational(1), "1/2 + 1/2 = 1");
    T.ok(rational(1, 2) * rational(2, 3) == rational(1, 3), "1/2 * 2/3 = 1/3");
    T.ok(rational(1) + rational(3) == rational(4), "1 + 3 = 4");
    T.ok(rational(4) * rational(2) == rational(8), "4 * 2 = 8");
    T.ok(rational(-1,-1) == rational(1), "-1/-1 = 1");
    T.ok(rational(2,-1) == rational(-2,1), "2/-1 = -2/1");
    T.ok(rational(-1,-2) == rational(1,2), "-1/-2 = 1/2");
    for (int i=0; i<10; i++) {
	rational a = rand_rational();
	cout << "a=" << a << endl;
    }
//    T.done();
}

// -------------------------------------------------------------------------

/**
Rational matrix class
*/
class RationalMatrix
{
public:
    unsigned int width;
    unsigned int height;
    rational* matrix;
    RationalMatrix():
	width {0},
	height {0},
	matrix {nullptr}
	{};
    /*
    // copy constructor
    RationalMatrix(const RationalMatrix &a)
	{
	    width = a.width;
	    height = a.height;
	    unsigned int size = width * height;
	    matrix = new rational[size];
	    memcpy (matrix, a.matrix, size * sizeof(rational));
	};
    */
    RationalMatrix(unsigned int N):
	width {N},
	height {N}
	{
	    unsigned int size = N * N;
	    matrix = new rational[size];
	};
    RationalMatrix(unsigned int Width, unsigned int Height):
	width {Width},
	height {Height}
	{
	    unsigned int size = Width * Height;
	    matrix = new rational[size];
	};
    ~RationalMatrix() {
	if (matrix) {
	    //delete[] matrix;
	    matrix = nullptr;
	}
	width = 0;
	height = 0;
    };
    void fillRandom()
	{
	    for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; i < width; i++) {
		    matrix[i*width + j] = rand_rational();
		}
	    }
	};
    void fill0() {
	for (unsigned int i = 0; i < height; i++) {
	    for (unsigned int j = 0; j < width; j++) {
		matrix[i*width + j] = 0;
	    }
	}
    }
    void fillE()
	{
	    fill0();
	    for (unsigned int i = 0; (i < height) && (i < width); i++) {
		matrix[i*width + i] = rational(1);
	    }
	};
    // new_matrix_from_int_array
    void fillFromIntArray(int* data)
	{
	    for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {
		    matrix[i*width + j] = data[i * width + j];
		}
	    }
	}
    void print() const {
	for (unsigned int j = 0; j < width; j++) {
	    cout << "--";
	}
	cout << endl;
	for (unsigned int i = 0; i < height; i++) {
	    for (unsigned int j = 0; j < width; j++) {
		cout << matrix[i*width + j] << " ";
	    }
	    cout << endl;
	}
	for (int j = 0; j < width; j++) {
	    cout << "--";
	}
	cout << endl;
    }

    RationalMatrix& operator=(const RationalMatrix& other);
    bool operator ==(RationalMatrix);
    RationalMatrix operator+(RationalMatrix);
    RationalMatrix operator-(RationalMatrix);
    RationalMatrix operator *(RationalMatrix);
    //friend inline void multiply_aij(RationalMatrix a, RationalMatrix b, RationalMatrix result, unsigned int i, unsigned int j);
    //friend inline void sequential_multiply(RationalMatrix a, RationalMatrix b, RationalMatrix result);
    //friend inline void parallel_multiply(RationalMatrix a, RationalMatrix b, RationalMatrix result);
};

/**
overloaded = operator
*/
RationalMatrix& RationalMatrix:: operator=(const RationalMatrix& other)
{
    // Guard self assignment
    if (this == &other)
        return *this;
 
    // assume *this manages a reusable resource, such as a heap-allocated buffer mArray
    if (width != other.width || height!=other.height)           // resource in *this cannot be reused
    {
	rational* temp = nullptr;
	if (other.matrix !=nullptr)
	    temp = new rational[other.width * other.height];   // allocate resource, if throws, do nothing
        if (matrix !=nullptr) delete[] matrix;              // release resource in *this
        matrix = temp;
	width = other.width;
	height = other.height;
    } 
 
    if (other.matrix !=nullptr)
	std::copy(other.matrix, other.matrix + other.width * other.height, matrix);
    return *this;
}

/**
overloaded == operator
*/
bool RationalMatrix ::operator ==(RationalMatrix b)
{
    if (width != b.width || height != b.height) return false;
    for (unsigned int i = 0; i < width * height; i++) {
	if (! (matrix[i] == b.matrix[i])) return false;
    }
    return true;
}

/**
overloaded + operator
*/
RationalMatrix RationalMatrix:: operator+(RationalMatrix b)
{
    RationalMatrix result(width, height);
    if (width != b.width || height != b.height) {
	cout << "Error: dimentions mismatch" << endl;
	return result;
    }
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            result.matrix[i*width + j] = matrix[i*width + j] + b.matrix[i*width + j];
        }
    }
    return result;
}

/**
overloaded - operator
*/
RationalMatrix RationalMatrix:: operator-(RationalMatrix b)
{
    RationalMatrix result(width, height);
    if (width != b.width || height != b.height) {
	cout << "Error: dimentions mismatch" << endl;
	return result;
    }
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            result.matrix[i*width + j] = matrix[i*width + j] - b.matrix[i*width + j];
        }
    }
    return result;
}

/**
overloaded * operator
*/
RationalMatrix RationalMatrix:: operator*(RationalMatrix b)
{
    RationalMatrix result(b.width, height);
    if (width != b.height) {
	cout << "Error: dimentions mismatch" << endl;
	return result;
    }
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < b.width; j++)
        {
            rational acc = 0;
            for (unsigned int k = 0; k < width; k++)
            {
                acc = acc + matrix[i*width + k] * b.matrix[k*b.width + j];
            }
	    result.matrix[i*result.width + j] = acc;
        }
    }
    return result;
}

/**
muliply function
*/
inline static
void multiply_aij(RationalMatrix a, RationalMatrix b, RationalMatrix result, unsigned int i, unsigned int j)
{
    rational acc = 0;
    for (unsigned int k = 0; k < a.width; k++)
    {
	acc = acc + a.matrix[i*a.width + k] * b.matrix[k*b.width + j];
    }
    result.matrix[i*result.width + j] = acc;
}

/**
sequential multiply
*/
inline static
void sequential_multiply(RationalMatrix a, RationalMatrix b, RationalMatrix result)
{
    if (a.width != b.height || b.width != result.width || a.height != result.height) {
	cout << "Error: dimentions mismatch" << endl;
	return;
    }
    for (unsigned int i = 0; i < a.height; i++)
    {
        for (unsigned int j = 0; j < b.width; j++)
        {
	    multiply_aij(a, b, result, i, j);
        }
    }
}

/**
subinterval of multithreaded multiplication
*/
static
void multiply_subinterval(RationalMatrix a, RationalMatrix b, RationalMatrix result, unsigned int from, unsigned int to, unsigned int max)
{
    for (unsigned int k = from; (k < to ) && (k < max) ; k++)
	{
	    //unsigned int i = k / b.width;
	    //unsigned int j = k % b.width;
	    //multiply_aij(a, b, result, i, j);
	    multiply_aij(a, b, result, k / b.width, k % b.width);
	}
}

/**
multithreaded multiply
*/
inline static
void parallel_multiply(RationalMatrix a, RationalMatrix b, RationalMatrix result, unsigned int num_threads)
{
    if (a.width != b.height || b.width != result.width || a.height != result.height) {
	cout << "Error: dimentions mismatch" << endl;
	return;
    }
    unsigned int matrix_length = a.height * b.width;
    unsigned int cells_per_thread = matrix_length / num_threads;
    if (matrix_length % num_threads) cells_per_thread++;

    std::vector<std::thread> threads_multiply;

    for (unsigned int thread_num = 0; thread_num < num_threads; thread_num++)
    {

	threads_multiply.push_back(
	    std::thread(
		&multiply_subinterval, a, b, result, thread_num*cells_per_thread, 
		(thread_num+1)*cells_per_thread, matrix_length
		)
	    );
    }

    for (auto& th : threads_multiply) {
        th.join();
    }
}


// -------------------------------------------------------------------------

/**
test matrix
*/
void test_matrix() {
    unittest T = unittest("matrix");
    int TestSize = 8;
    int matrix_ints_array[64] = {
    1,0,0,0,1,2,1,5,
    0,2,0,0,0,7,3,4,
    0,0,3,0,0,0,2,6,
    0,0,0,4,0,0,0,9,
    0,0,0,0,5,0,0,0,
    0,0,0,0,0,4,0,0,
    0,0,0,0,0,0,3,0,
    0,0,0,0,0,0,0,2,
    };


    RationalMatrix TE(TestSize);
    TE.fillE();
    RationalMatrix Tout = TE * TE;
    T.ok(TE == Tout);
    TE.print();

    RationalMatrix T1(TestSize);
    T1.fillFromIntArray(matrix_ints_array);
    RationalMatrix T0 = T1 * T1;
    T1.print();


    RationalMatrix T2 = T1 * TE;
    T.ok(T1 == T2);

    RationalMatrix RAND(TestSize);
    RAND.fillRandom();
    RAND.print();
    Tout = RAND * TE;
    T.ok(RAND == Tout);

    //odd-number matrix:

    int TestOddSize = 9;
    RationalMatrix T_ODD_E(TestOddSize);
    T_ODD_E.fillE();
    int matrix_ints_array_odd[81] = {
    1,0,0,0,1,2,1,5,2,
    0,2,0,0,0,7,3,4,1,
    0,0,3,0,0,0,2,6,9,
    0,0,0,4,0,0,0,9,6,
    0,0,0,0,5,0,0,0,0,
    0,0,0,0,0,4,0,0,0,
    0,0,0,0,0,0,3,0,0,
    0,0,0,0,0,0,0,2,0,
    0,0,0,0,0,0,0,0,3,
    };

    RationalMatrix a(TestOddSize);
    RationalMatrix result(TestOddSize);
    a.fillE();
    result.fill0();
    sequential_multiply(a, a, result);
    T.ok(result == a);
    result.fill0();
    parallel_multiply(a, a, result, 4);
    result.print();
    T.ok(result == a);
}

// -------------------------------------------------

/**
benchmarking function, returns shortest function execution time out of 10000 tests
*/
double benchmark(void (*funcptr)(unsigned int), unsigned int num_threads) {
    //test the function 10000 times

    std::chrono::steady_clock::time_point _start;
    std::chrono::steady_clock::time_point _end;
    double duration;
    double lowest_duration=1e40;//set to really high amount of seconds for now, hacky workaround

    for (int i = 0; i < 10000; i++) {
        _start = std::chrono::steady_clock::now();
        funcptr(num_threads);
        _end = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point _end(std::chrono::steady_clock::now());
        duration = std::chrono::duration_cast<std::chrono::duration<double>>(_end - _start).count();
        duration = duration * 1000;//miliseconds

        if (duration < lowest_duration) {
            lowest_duration = duration;
        }
    }
    return lowest_duration;
}

/**
benchmark sequential matrix mult, with a 24x24 matrix
*/
void benchmark_part_sequential(unsigned int num_threads) {
    RationalMatrix a(24);
    RationalMatrix result(24);
    a.fillE();
    sequential_multiply(a, a, result);
}

/**
benchmark multithread matrix mult, with a 24x24 matrix
*/
void benchmark_part_multithread(unsigned int num_threads) {
    RationalMatrix a(24);
    RationalMatrix result(24);
    a.fillE();
    parallel_multiply(a, a, result, num_threads);
}



// -------------------------------------------------

/**
main function
*/
int main() {
    //initialize random seed:
    srand(time(NULL));
    test_rational();
    test_matrix();

    std::cout << "sequentical matrix mult benchmark in miliseconds (lowest duration): " << benchmark(benchmark_part_sequential, 0) << std::endl;

    std::cout << "multithread matrix mult benchmark in miliseconds (1 thread): " << benchmark(benchmark_part_multithread, 1) << std::endl;

    std::cout << "multithread matrix mult benchmark in miliseconds (2 threads): " << benchmark(benchmark_part_multithread, 2) << std::endl;

    std::cout << "multithread matrix mult benchmark in miliseconds (4 threads): " << benchmark(benchmark_part_multithread, 4) << std::endl;

    std::cout << "multithread matrix mult benchmark in miliseconds (8 threads): " << benchmark(benchmark_part_multithread, 8) << std::endl;

    return 0;
}
