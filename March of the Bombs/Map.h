#pragma once

#include <boost/multi_array.hpp>

#include "Block.h"
#include "PathMap.h"
#include "Graphics.h"

/**
 * Map represents the map in terms of blocks, bases and the ground plane.
 */
class Map
{
private:
	/// A boost multi-array with two dimensions, kinda weird usage
	typedef boost::multi_array<Block::ptr, 2> block2DArray;
	block2DArray blockMap;
	/// Locations of the bases
	std::vector<glm::ivec2> bases;
	/// Map for path finding
	PathMap pathMap;

	/// Size of the map, in blocks
	glm::ivec2 size;

	Model::ptr groundPlane;

	/// Load a default map (for testing)
	void loadDefaultMap();
	/**
	 * Load the specified map. If the file could not be opened, the map
	 * will remain as before.
	 */
	void loadMapFromFile(string const& filename);

public:
	/**
	 * Constructor. Attempts to load the map with the given filename.
	 * Will fail silently if the file could not be opened, in which case the size will be 0, 0.
	 */
	Map(std::string const& fileName);

	/**
	 * Find a path from start to goal and store the result in path. Return true on success and false
	 * on failure. Will always remove any old path. Fails if there is no path from path to goal,
	 * including if either is in a blocked location.
	 */
	bool findPath(glm::vec3 const& start, glm::vec3 const& goal, std::list<glm::vec3>& path) const;

	/// Draw the blocks and the plane
	void draw(Graphics::ptr graphics);
	/**
	 * Return true if a ray cast from origin in the direction direction hits the ground plane in
	 * less than the value in distance. If so, the value in distance is updated to the distance
	 * to the intersection.
	 */
	bool intersectGround(glm::vec3 const& origin, glm::vec3 const& direction, float& distance) const;

	/// Return the location of the bases
	std::vector<glm::ivec2> const& getBases() const;
	/// Return the block at the specified position, or a empty pointer if there is no block there
	Block::ptr getBlock(glm::ivec2 const& pos) const;
	/// Replace the specified block with a EmptyBlock. block must be valid.
	void removeBlock(glm::ivec2 const& block);

	/// Get the size of the current map (in blocks)
	glm::ivec2 getSize() const;
};