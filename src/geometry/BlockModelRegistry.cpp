#include "../data/BlockModel.h"

BlockModel* BlockModelRegistry::loadModel(const std::string& modelName) {
	// Will load the model from the assets folder into a BlockModel
	return nullptr;
}

BlockModel* BlockModelRegistry::getModel(const std::string& modelName) {
	auto it = models.find(modelName);
	if (it != models.end()) {
		return &it->second;
	}
	return nullptr;
}

bool BlockModelRegistry::hasModel(const std::string& modelName) const {
	return models.find(modelName) != models.end();
}

void BlockModelRegistry::createDefaultCubeModel() {
	BlockModel cubeModel;
	cubeModel.modelName = "cube";
	cubeModel.isFullCube = true;
	cubeModel.boundsMin = Vec3(0, 0, 0);
	cubeModel.boundsMax = Vec3(1, 1, 1);

	cubeModel.faceTextures["north"] = "default";
	cubeModel.faceTextures["south"] = "default";
	cubeModel.faceTextures["east"] = "default";
	cubeModel.faceTextures["west"] = "default";
	cubeModel.faceTextures["top"] = "default";
	cubeModel.faceTextures["bottom"] = "default";

	models["cube"] = cubeModel;
}

uint32_t readVarint(const std::string& data, size_t& pos) {
	uint32_t result = 0;
	int shift = 0;
	while (pos < data.size()) {
		uint8_t byte = static_cast<uint8_t>(data[pos]);
		pos++;
		result |= (byte & 0x7F) << shift;
		if ((byte & 0x80) == 0) break;
		shift += 7;
	}
	return result;
}

void BlockIDMappings::parseBlockList(const std::string& data) {
	std::unordered_map<BlockID, std::string> list;
	size_t pos = 0;

	while (pos < data.size()) {
		if (static_cast<uint8_t>(data[pos]) != 0x0A) {
			break;
		}
		pos++;

		uint32_t entryLen = readVarint(data, pos);
		size_t entryEnd = pos + entryLen;

		uint32_t blockId = 0;
		std::string blockName;

		while (pos < entryEnd) {
			uint8_t tag = static_cast<uint8_t>(data[pos]);
			pos++;

			if (tag == 0x08) {
				blockId = readVarint(data, pos);
			}
			else if (tag == 0x12) {
				uint32_t strLen = readVarint(data, pos);
				blockName = data.substr(pos, strLen);
				pos += strLen;
			}
		}

		list[blockId] = blockName;
	}

	blockNames = list;
}

std::string BlockIDMappings::getBlockName(const BlockID& blockId) {
	return "";
}