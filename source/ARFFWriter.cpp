#include "ARFFWriter.h"

// constructor
// formats the output file to ARFF file format
// @ATTRIBUTE are all the numeric values from the leapData
// the for loop goes through the data vector
ARFFWriter::ARFFWriter(std::string path, std::vector<LeapData> data) {
	std::ofstream file;
	file.open(path);
	file << "% 1. Title: Leap Data Database\n" <<
		"%\n" <<
		"% 2. Sources :\n" <<
		"% (a)Creator : Giulio Marin, Fabio Dominio, Pietro Zanuttigh, and Binghamton SRI 2016\n" <<
		"% (b)Date : Summer 2016\n" <<
		"%\n" <<
		"@RELATION gesture\n" <<
		"\n" <<
		"@ATTRIBUTE numFingers  NUMERIC\n" <<
		"@ATTRIBUTE fingersExtended   NUMERIC\n" <<
		"@ATTRIBUTE ratio   NUMERIC\n" <<
		"@ATTRIBUTE Max_X   NUMERIC\n" <<
		"@ATTRIBUTE Max_Y   NUMERIC\n" <<
		"@ATTRIBUTE fingersAreas   NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f1  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f2  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f3  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f4  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f5  NUMERIC\n" <<
		"@ATTRIBUTE fingertipAngles_f1   NUMERIC\n" <<
		"@ATTRIBUTE fingertipAngles_f2   NUMERIC\n" <<
		"@ATTRIBUTE fingertipAngles_f3   NUMERIC\n" <<
		"@ATTRIBUTE fingertipAngles_f4   NUMERIC\n" <<
		"@ATTRIBUTE fingertipAngles_f5   NUMERIC\n" <<
		"@ATTRIBUTE fingertipElevation_f1   NUMERIC\n" <<
		"@ATTRIBUTE fingertipElevation_f2   NUMERIC\n" <<
		"@ATTRIBUTE fingertipElevation_f3   NUMERIC\n" <<
		"@ATTRIBUTE fingertipElevation_f4   NUMERIC\n" <<
		"@ATTRIBUTE fingertipElevation_f5   NUMERIC\n" <<
		"@ATTRIBUTE class { G1, G2, G3, G4, G5, G6, G7, G8, G9, G0 }\n" <<
		"\n@DATA\n";

		// goes through the data LeapData vector and gets the values for the arff file
		for (int i = 0; i < data.size(); i++) {
			file << getNumFingers(data[i]) << getFingersExtends(data[i]) << getRatio(data[i]) << getMax_X(data[i]) << getMax_Y(data[i]) << getFingersArea(data[i]) << getNewFingertipDistRefined(data[i]) << getFingertipAngles(data[i]) << getFingertipElevation(data[i]) << getGesture(data[i]) << "\n";
		}

	file.close();
}
// returns values as strings

ARFFWriter::ARFFWriter(std::string path, std::vector<KinectMotion> data)
{
	std::ofstream file;
	file.open(path);
	file << "% 1. Title: Leap Data Database\n" <<
		"%\n" <<
		"% 2. Sources :\n" <<
		"% (a)Creator : Giulio Marin, Fabio Dominio, Pietro Zanuttigh, and Binghamton SRI 2016\n" <<
		"% (b)Date : Summer 2016\n" <<
		"%\n" <<
		"@RELATION gesture\n" <<
		"\n" <<
		"@ATTRIBUTE silhouette1  NUMERIC\n" <<
		"@ATTRIBUTE silhouette2  NUMERIC\n" <<
		"@ATTRIBUTE silhouette3  NUMERIC\n" <<
		"@ATTRIBUTE silhouette4  NUMERIC\n" <<
		"@ATTRIBUTE silhouette5  NUMERIC\n" <<
		"@ATTRIBUTE silhouette6  NUMERIC\n" <<
		"@ATTRIBUTE silhouette7  NUMERIC\n" <<
		"@ATTRIBUTE silhouette8  NUMERIC\n" <<
		"@ATTRIBUTE silhouette9  NUMERIC\n" <<
		"@ATTRIBUTE silhouette10  NUMERIC\n" <<
		"@ATTRIBUTE silhouette11  NUMERIC\n" <<
		"@ATTRIBUTE silhouette12  NUMERIC\n" <<
		"@ATTRIBUTE silhouette13  NUMERIC\n" <<
		"@ATTRIBUTE silhouette14  NUMERIC\n" <<
		"@ATTRIBUTE silhouette15  NUMERIC\n" <<
		"@ATTRIBUTE silhouette16  NUMERIC\n" <<
		"@ATTRIBUTE silhouette17  NUMERIC\n" <<
		"@ATTRIBUTE silhouette18  NUMERIC\n" <<
		"@ATTRIBUTE silhouette19  NUMERIC\n" <<
		"@ATTRIBUTE silhouette20  NUMERIC\n" <<
		"@ATTRIBUTE silhouette21  NUMERIC\n" <<
		"@ATTRIBUTE silhouette22  NUMERIC\n" <<
		"@ATTRIBUTE silhouette23  NUMERIC\n" <<
		"@ATTRIBUTE silhouette24  NUMERIC\n" <<
		"@ATTRIBUTE silhouette25  NUMERIC\n" <<
		"@ATTRIBUTE silhouette26  NUMERIC\n" <<
		"@ATTRIBUTE silhouette27  NUMERIC\n" <<
		"@ATTRIBUTE silhouette28  NUMERIC\n" <<
		"@ATTRIBUTE silhouette29  NUMERIC\n" <<
		"@ATTRIBUTE silhouette30  NUMERIC\n" <<
		"@ATTRIBUTE silhouette31  NUMERIC\n" <<
		"@ATTRIBUTE silhouette32  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances1  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances2  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances3  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances4  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances5  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances6  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances7  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances8  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances9  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances10  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances11  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances12  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances13  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances14  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances15  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances16  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances17  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances18  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances19  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances20  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances21  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances22  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances23  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances24  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances25  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances26  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances27  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances28  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances29  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances30  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances31  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances32  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances33  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances34  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances35  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances36  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances37  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances38  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances39  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances40  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances41  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances42  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances43  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances44  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances45  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances46  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances47  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances48  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances49  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances50  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances51  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances52  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances53  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances54  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances55  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances56  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances57  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances58  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances59  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances60  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances61  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances62  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances63  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances64  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances65  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances66  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances67  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances68  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances69  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances70  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances71  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances72  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances73  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances74  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances75  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances76  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances77  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances78  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances79  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances80  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances81  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances82  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances83  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances84  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances85  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances86  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances87  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances88  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances89  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances90  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances91  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances92  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances93  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances94  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances95  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances96  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances97  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances98  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances99  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances100  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances101  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances102  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances103  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances104  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances105  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances106  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances107  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances108  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances109  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances110  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances111  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances112  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances113  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances114  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances115  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances116  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances117  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances118  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances119  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances120  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances121  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances122  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances123  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances124  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances125  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances126  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances127  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances128  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances129  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances130  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances131  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances132  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances133  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances134  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances135  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances136  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances137  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances138  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances139  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances140  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances141  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances142  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances143  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances144  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances145  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances146  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances147  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances148  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances149  NUMERIC\n" <<
		"@ATTRIBUTE contourDistances150  NUMERIC\n" <<
		"@ATTRIBUTE convexHull1  NUMERIC\n" <<
		"@ATTRIBUTE convexHull2  NUMERIC\n" <<
		"@ATTRIBUTE convexHull3  NUMERIC\n" <<
		"@ATTRIBUTE convexHull4  NUMERIC\n" <<
		"@ATTRIBUTE convexHull5  NUMERIC\n" <<
		"@ATTRIBUTE convexHull6  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ1  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ2  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ3  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ4  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ5  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ6  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ7  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ8  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ9  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ10  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ11  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ12  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ13  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ14  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ15  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ16  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ17  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ18  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ19  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ20  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ21  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ22  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ23  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ24  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ25  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ26  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ27  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ28  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ29  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ30  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ31  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ32  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ33  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ34  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ35  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ36  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ37  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ38  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ39  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ40  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ41  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ42  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ43  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ44  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ45  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ46  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ47  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ48  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ49  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ50  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ51  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ52  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ53  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ54  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ55  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ56  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ57  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ58  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ59  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ60  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ61  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ62  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ63  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyNonZ64  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg1  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg2  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg3  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg4  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg5  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg6  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg7  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg8  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg9  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg10  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg11  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg12  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg13  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg14  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg15  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg16  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg17  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg18  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg19  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg20  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg21  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg22  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg23  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg24  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg25  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg26  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg27  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg28  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg29  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg30  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg31  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg32  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg33  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg34  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg35  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg36  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg37  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg38  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg39  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg40  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg41  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg42  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg43  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg44  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg45  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg46  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg47  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg48  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg49  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg50  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg51  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg52  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg53  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg54  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg55  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg56  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg57  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg58  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg59  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg60  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg61  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg62  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg63  NUMERIC\n" <<
		"@ATTRIBUTE cellOccupancyAvg64  NUMERIC\n" <<
		"@ATTRIBUTE fingerAngle1  NUMERIC\n" <<
		"@ATTRIBUTE fingerAngle2  NUMERIC\n" <<
		"@ATTRIBUTE fingerAngle3  NUMERIC\n" <<
		"@ATTRIBUTE fingerAngle4  NUMERIC\n" <<
		"@ATTRIBUTE fingerAngle5  NUMERIC\n" <<
		"@ATTRIBUTE fingerDist1  NUMERIC\n" <<
		"@ATTRIBUTE fingerDist2  NUMERIC\n" <<
		"@ATTRIBUTE fingerDist3  NUMERIC\n" <<
		"@ATTRIBUTE fingerDist4  NUMERIC\n" <<
		"@ATTRIBUTE fingerDist5  NUMERIC\n" <<
		"@ATTRIBUTE class { G1, G2, G3, G4, G5, G6, G7, G8, G9, G0 }\n" <<
		"\n@DATA\n";

	// goes through the data LeapData vector and gets the values for the arff file
	for (int i = 0; i < data.size(); i++) {
		file << getSil(data[i]) << getContourDist(data[i]) << getHull(data[i]) << getOccNonz(data[i]) << getOccAvg(data[i]) << getFingerAngle(data[i]) << getFingerDist(data[i]) << getGesture(data[i]) << "\n";
	}

	file.close();
}

// constructor for just the new leap features
ARFFWriter::ARFFWriter(std::string path, std::vector<LeapData> data, bool val) {
	std::ofstream file;
	file.open(path);
	file << "% 1. Title: Leap Data Database\n" <<
		"%\n" <<
		"% 2. Sources :\n" <<
		"% (a)Creator : Giulio Marin, Fabio Dominio, Pietro Zanuttigh, and Binghamton SRI 2016\n" <<
		"% (b)Date : Summer 2016\n" <<
		"%\n" <<
		"@RELATION gesture\n" <<
		"\n" <<
		"@ATTRIBUTE numFingers  NUMERIC\n" <<
		"@ATTRIBUTE fingersExtended   NUMERIC\n" <<
		"@ATTRIBUTE ratio   NUMERIC\n" <<
		"@ATTRIBUTE Max_X   NUMERIC\n" <<
		"@ATTRIBUTE Max_Y   NUMERIC\n" <<
		"@ATTRIBUTE fingersAreas   NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f1  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f2  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f3  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f4  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f5  NUMERIC\n" <<
		"@ATTRIBUTE class { G1, G2, G3, G4, G5, G6, G7, G8, G9, G0 }\n" <<
		"\n@DATA\n";

	// goes through the data LeapData vector and gets the values for the arff file
	for (int i = 0; i < data.size(); i++) {
		file << getNumFingers(data[i]) << getFingersExtends(data[i]) << getRatio(data[i]) << getMax_X(data[i]) << getMax_Y(data[i]) << getFingersArea(data[i]) << getNewFingertipDistRefined(data[i]) << getGesture(data[i]) << "\n";
	}

	file.close();
}

std::string ARFFWriter::getNumFingers(LeapData leap) {
	return std::to_string(leap.getNumFingers()) + ",";
}

std::string ARFFWriter::getFingertipDist(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingertipDist().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingertipDist()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getFingertipDistRefined(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingertipDistRefined().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingertipDistRefined()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getFingertipInterDist(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingertipInterDist().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingertipInterDist()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getFingertipPosition(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingertipPosition().size(); i++) {
		returnVal = returnVal + leap.getFingertipPosition()[i].writePoint() + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getHandDirection(LeapData leap) {
	return leap.getHandDirection().writePoint() + ",";
}
std::string ARFFWriter::getHandSphereCenter(LeapData leap) {
	return leap.getHandSphereCenter().writePoint() + ",";
}
std::string ARFFWriter::getHandSphereRadius(LeapData leap) {
	return std::to_string(leap.getHandSphereRadius()) + ",";
}
std::string ARFFWriter::getPalmNormal(LeapData leap) {
	return leap.getPalmNormal().writePoint() + ",";
}
std::string ARFFWriter::getPalmPosition(LeapData leap) {
	return leap.getPalmPosition().writePoint() + ",";
}
std::string ARFFWriter::getPalmPositionRefined(LeapData leap) {
	return leap.getPalmPositionRefined().writePoint() + ",";
}
std::string ARFFWriter::getPalmVelocity(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getPalmVelocity().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getPalmVelocity()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getRotationAngle(LeapData leap) {
	return std::to_string(leap.getRotationAngle()) + ",";
}
std::string ARFFWriter::getRotationAxis(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getRotationAxis().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getRotationAxis()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getRotationMatrix(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getRotationMatrix().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getRotationMatrix()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getRotationProbability(LeapData leap) {
	return std::to_string(leap.getRotationProbability()) + ",";
}
std::string ARFFWriter::getTranslation(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getTranslation().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getTranslation()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getTranslationProbability(LeapData leap) {
	return std::to_string(leap.getTranslationProbability()) + ",";
}

//new attributes
std::string ARFFWriter::getNewScaleFactor(LeapData leap) {
	return std::to_string(leap.getNewScaleFactor()) + ",";
}
std::string ARFFWriter::getNewFingertipDistRefined(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getNewFingertipDistRefined().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getNewFingertipDistRefined()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getFingertipAngles(LeapData leap) {
	std::string returnVal;

	for (int i = 0; i < leap.getFingertipAngles().size(); i++) {

		returnVal = returnVal + std::to_string(leap.getFingertipAngles()[i]) + ",";
	}

	return returnVal;
}
std::string ARFFWriter::getProjectionPoints(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getProjectionPoints().size(); i++) {
		returnVal = returnVal + leap.getProjectionPoints()[i].writePoint() + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getFingertipElevation(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingertipElevation().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingertipElevation()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getGesture(LeapData leap) {
	return leap.getGesture();
}

std::string ARFFWriter::getFingersExtends(LeapData leap) {
	std::vector<int> extend = leap.getExtendedFingers();
	std::string returnVal;
	for (int i = 0; i < extend.size(); i++) {
		returnVal = returnVal + std::to_string(extend[i]);
	}
	returnVal.push_back(',');
	return returnVal;
}
std::string ARFFWriter::getFingersArea(LeapData leap)
{
	return std::to_string(leap.getFingerArea()) + ",";
}

//Kinect
std::string ARFFWriter::getSil(KinectMotion depth)
{
	float * silVals = depth.getSil();
	std::string returnVal = "";
	for (int i = 0; i < 32; i++) {
		returnVal += std::to_string(silVals[i]);
		returnVal += ", ";
	}
	return returnVal;
}

std::string ARFFWriter::getContourDist(KinectMotion depth)
{
	float * contourDistVals = depth.getContourDist();
	std::string returnVal = "";
	for (int i = 0; i < 150; i++) {
		returnVal += std::to_string(contourDistVals[i]);
		returnVal += ", ";
	}
	return returnVal;
}

std::string ARFFWriter::getHull(KinectMotion depth)
{
	float * hullVals = depth.getHull();
	std::string returnVal = "";
	for (int i = 0; i < 6; i++) {
		returnVal += std::to_string(hullVals[i]);
		returnVal += ", ";
	}
	return returnVal;
}

std::string ARFFWriter::getOccNonz(KinectMotion depth)
{
	int * nonzVals = depth.getOccNonz();
	std::string returnVal = "";
	for (int i = 0; i < 64; i++) {
		returnVal += std::to_string(nonzVals[i]);
		returnVal += ", ";
	}
	return returnVal;
}

std::string ARFFWriter::getOccAvg(KinectMotion depth)
{
	float * avgVals = depth.getOccAvg();
	std::string returnVal = "";
	for (int i = 0; i < 64; i++) {
		returnVal += std::to_string(avgVals[i]);
		returnVal += ", ";
	}
	return returnVal;
}

std::string ARFFWriter::getFingerAngle(KinectMotion depth)
{
	float * angVals = depth.getFingerAngle();
	std::string returnVal = "";
	for (int i = 0; i < 5; i++) {
		returnVal += std::to_string(angVals[i]);
		returnVal += ", ";
	}
	return returnVal;
}

std::string ARFFWriter::getFingerDist(KinectMotion depth)
{
	float * distVals = depth.getFingerDist();
	std::string returnVal = "";
	for (int i = 0; i < 5; i++) {
		returnVal += std::to_string(distVals[i]);
		returnVal += ", ";
	}
	return returnVal;
}

std::string ARFFWriter::getGesture(KinectMotion Kinect) {
	return Kinect.getGesture();
}

std::string ARFFWriter::getRatio(LeapData leap)
{
	return std::to_string(leap.getRatio()) + ",";
}
std::string ARFFWriter::getMax_X(LeapData leap)
{
	return std::to_string(leap.getMax_X()) + ",";
}
std::string ARFFWriter::getMax_Y(LeapData leap)
{
	return std::to_string(leap.getMax_Y()) + ",";
}