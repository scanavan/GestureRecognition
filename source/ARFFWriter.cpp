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
		/*"@ATTRIBUTE fingerTipDist_f1   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipDist_f2   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipDist_f3   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipDist_f4   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipDist_f5   NUMERIC\n" <<*/
		"@ATTRIBUTE newFingerTipDistRefined_f1  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f2  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f3  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f4  NUMERIC\n" <<
		"@ATTRIBUTE newFingerTipDistRefined_f5  NUMERIC\n" <<
		/*"@ATTRIBUTE fingertipInterDist_d1   NUMERIC\n" <<
		"@ATTRIBUTE fingertipInterDist_d2   NUMERIC\n" <<
		"@ATTRIBUTE fingertipInterDist_d3   NUMERIC\n" <<
		"@ATTRIBUTE fingertipInterDist_d4   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f1x  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f1y  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f1z  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f2x  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f2y  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f2z  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f3x  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f3y  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f3z  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f4x  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f4y  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f4z  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f5x  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f5y  NUMERIC\n" <<
		"@ATTRIBUTE fingerTipPosition_f5z  NUMERIC\n" <<
		"@ATTRIBUTE handDirection_x   NUMERIC\n" <<
		"@ATTRIBUTE handDirection_y   NUMERIC\n" <<
		"@ATTRIBUTE handDirection_z   NUMERIC\n" <<
		"@ATTRIBUTE handSphereCenter_x  NUMERIC\n" <<
		"@ATTRIBUTE handSphereCenter_y  NUMERIC\n" <<
		"@ATTRIBUTE handSphereCenter_z  NUMERIC\n" <<
		"@ATTRIBUTE handSphereRadius   NUMERIC\n" <<
		"@ATTRIBUTE palmNormal_x  NUMERIC\n" <<
		"@ATTRIBUTE palmNormal_y  NUMERIC\n" <<
		"@ATTRIBUTE palmNormal_z  NUMERIC\n" <<
		"@ATTRIBUTE palmPosition_x   NUMERIC\n" <<
		"@ATTRIBUTE palmPosition_y   NUMERIC\n" <<
		"@ATTRIBUTE palmPosition_z   NUMERIC\n" <<
		"@ATTRIBUTE palmPositionRefined_x  NUMERIC\n" <<
		"@ATTRIBUTE palmPositionRefined_y  NUMERIC\n" <<
		"@ATTRIBUTE palmPositionRefined_z  NUMERIC\n" <<
		"@ATTRIBUTE palmVelocity_x   NUMERIC\n" <<
		"@ATTRIBUTE palmVelocity_y   NUMERIC\n" <<
		"@ATTRIBUTE palmVelocity_z   NUMERIC\n" <<
		"@ATTRIBUTE rotationAngle  NUMERIC\n" <<
		"@ATTRIBUTE rotationAxis_x   NUMERIC\n" <<
		"@ATTRIBUTE rotationAxis_y   NUMERIC\n" <<
		"@ATTRIBUTE rotationAxis_z   NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_1x  NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_1y  NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_1z  NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_2x  NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_2y  NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_2z  NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_3x  NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_3y  NUMERIC\n" <<
		"@ATTRIBUTE rotationMatrix_3z  NUMERIC\n" <<
		"@ATTRIBUTE rotationProbability   NUMERIC\n" <<
		"@ATTRIBUTE newScaleFactor  NUMERIC\n" <<
		"@ATTRIBUTE translation_x   NUMERIC\n" <<
		"@ATTRIBUTE translation_y   NUMERIC\n" <<
		"@ATTRIBUTE translation_z   NUMERIC\n" <<
		"@ATTRIBUTE translationProbability  NUMERIC\n" <<*/
		"@ATTRIBUTE fingerTipAngles_f1   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipAngles_f2   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipAngles_f3   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipAngles_f4   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipAngles_f5   NUMERIC\n" <<
		/*"@ATTRIBUTE projectionPoints_f1x   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f1y   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f1z   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f2x   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f2y   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f2z   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f3x   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f3y   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f3z   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f4x   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f4y   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f4z   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f5x   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f5y   NUMERIC\n" <<
		"@ATTRIBUTE projectionPoints_f5z   NUMERIC\n" <<*/
		"@ATTRIBUTE fingerTipElevation_f1   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipElevation_f2   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipElevation_f3   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipElevation_f4   NUMERIC\n" <<
		"@ATTRIBUTE fingerTipElevation_f5   NUMERIC\n" <<
		"@ATTRIBUTE class { G0, G1, G2, G6, G7, G9 }\n" <<
		"\n@DATA\n";

	for (int i = 0; i < data.size(); i++) {
		file << getNumFingers(data[i]) << getNewFingerTipDistRefined(data[i]) <<getFingerTipAngles(data[i]) << getFingerTipElevation(data[i]) << getGesture(data[i]) << "\n";
	}

	file.close();
}

std::string ARFFWriter::writeData(LeapData leap) {
	std::string returnVal;
	returnVal = returnVal + std::to_string(leap.getNumFingers()) + ",";

	for (int i = 0; i < leap.getFingerTipDist().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingerTipDist()[i]) + ",";
	}

	for (int i = 0; i < leap.getNewFingerTipDistRefined().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getNewFingerTipDistRefined()[i]) + ",";
	}

	for (int i = 0; i < leap.getFingerTipInterDist().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingerTipInterDist()[i]) + ",";
	}

	for (int i = 0; i < leap.getFingerTipPosition().size(); i++) {
		returnVal = returnVal + leap.getFingerTipPosition()[i].writePoint() + ",";
	}

	returnVal = returnVal + leap.getHandDirection().writePoint() + ",";

	returnVal = returnVal + leap.getHandSphereCenter().writePoint() + ",";

	returnVal = returnVal + std::to_string(leap.getHandSphereRadius()) + ",";

	returnVal = returnVal + leap.getPalmNormal().writePoint() + ",";

	returnVal = returnVal + leap.getPalmPosition().writePoint() + ",";


	returnVal = returnVal + leap.getPalmPositionRefined().writePoint() + ",";


	for (int i = 0; i < leap.getPalmVelocity().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getPalmVelocity()[i]) + ",";
	}

	returnVal = returnVal + std::to_string(leap.getRotationAngle()) + ",";


	for (int i = 0; i < leap.getRotationAxis().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getRotationAxis()[i]) + ",";
	}

	for (int i = 0; i < leap.getRotationMatrix().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getRotationMatrix()[i]) + ",";
	}

	returnVal = returnVal + std::to_string(leap.getRotationProbability()) + ",";

	returnVal = returnVal + std::to_string(leap.getNewScaleFactor()) + ",";

	for (int i = 0; i < leap.getTranslation().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getTranslation()[i]) + ",";
	}

	returnVal = returnVal + std::to_string(leap.getTranslationProbability()) + ",";

	for (int i = 0; i < leap.getFingerTipAngles().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingerTipAngles()[i]) + ",";
	}

	for (int i = 0; i < leap.getProjectionPoints().size(); i++) {
		returnVal = returnVal + leap.getProjectionPoints()[i].writePoint() + ",";
	}

	for (int i = 0; i < leap.getFingerTipElevation().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingerTipElevation()[i]) + ",";
	}
	returnVal = returnVal + "?";
		//leap.getGesture();

	return returnVal;
}

std::string ARFFWriter::getNumFingers(LeapData leap) {
	return std::to_string(leap.getNumFingers()) + ",";
}
std::string ARFFWriter::getFingerTipDist(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingerTipDist().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingerTipDist()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getFingerTipInterDist(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingerTipInterDist().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingerTipInterDist()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getFingerTipPosition(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingerTipPosition().size(); i++) {
		returnVal = returnVal + leap.getFingerTipPosition()[i].writePoint() + ",";
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
std::string ARFFWriter::getNewFingerTipDistRefined(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getNewFingerTipDistRefined().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getNewFingerTipDistRefined()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getFingerTipAngles(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingerTipAngles().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingerTipAngles()[i]) + ",";
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
std::string ARFFWriter::getFingerTipElevation(LeapData leap) {
	std::string returnVal;
	for (int i = 0; i < leap.getFingerTipElevation().size(); i++) {
		returnVal = returnVal + std::to_string(leap.getFingerTipElevation()[i]) + ",";
	}
	return returnVal;
}
std::string ARFFWriter::getGesture(LeapData leap) {
	return leap.getGesture();
}

