#pragma once

#include <boost\shared_ptr.hpp>

#include "Model.h"
#include "BlockModelData.h"
#include "Graphics.h"

/**
 * Base class for different blocks.
 */
class Block
{
protected:
	Model::ptr model;

	/// Constructor.
	Block(Model::ptr const& model);

public:
	typedef boost::shared_ptr<Block> ptr;

	/// Destructor.
	virtual ~Block();

	/// Empty update
	virtual void update(float deltaTime);
	/// Draw the model, if any
	virtual void draw(Graphics::ptr graphics);

	/// Return whether the block should be destroyed be an explosion or not
	virtual bool isDestructible() const;
};