int testverbose = 0;

using namespace std;

class unittest
{

	int testcount;
	int passed;
	int failed;
	string testname;
public:
	unittest(string testName) {
		testcount = 0;
		passed = 0;
		failed = 0;
		testname = testName;
	};
	~unittest() {
		cout << "test " << testname << " ";
		if (30 - testname.size() > 0) {
			for (unsigned int i = 0; i < 30 - testname.size(); i++)
				cout << ".";
		}
		cout << passed << "/" << testcount;
		if (failed) {
			cout << " failed" << endl;
		}
		else {
			cout << " OK" << endl;
		}
	}
	void ok(bool arg) {
		testcount++;
		if (arg) {
			passed++;
			if (testverbose)
				cout << "test " << testname << " " << testcount << " passed." << endl;
		}
		else {
			failed++;
			//if (testverbose)
				cout << "test " << testname << " " << testcount << " failed." << endl;
		}
	}
	void ok(bool arg,string message) {
		testcount++;
		if (arg) {
			passed++;
			if (testverbose)
				cout << "test " << testname << " " << testcount << " [" << message << "] passed." << endl;
		}
		else {
			failed++;
			if (testverbose)
				cout << "test " << testname << " " << testcount << " [" << message << "] failed." << endl;
		}
	}
	//void done() {};
};
