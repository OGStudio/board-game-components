FEATURE main.h/Impl
private:
    const std::string nodeSelectionCallbackName = "NodeSelection";
    const unsigned int selectionNodeMask = 0x00000004;
    osg::Node *selectedNode;
    core::Reporter selectedNodeChanged;

    void setupNodeSelection()
    {
        // Mark nodes as selectable by excluding specific bitmask.
        auto nodesCount = this->tileScene->getNumChildren();
        for (int id = 0; id < nodesCount; ++id)
        {
            auto node = this->tileScene->getChild(id);
            node->setNodeMask(node->getNodeMask() & ~this->selectionNodeMask);
        }

        // Listen to mouse clicks.
        this->app->mouse->pressedButtonsChanged.addCallback(
            [&] {
                // Try to select a node upon a click or a tap.
                bool clicked = !this->app->mouse->pressedButtons.empty();
                if (clicked)
                {
                    this->selectNode();
                }
            },
            this->nodeSelectionCallbackName
        );
    }
    void tearNodeSelectionDown()
    {
        this->app->mouse->pressedButtonsChanged.removeCallback(
            this->nodeSelectionCallbackName
        );
    }
    void selectNode()
    {
        auto node =
            scene::nodeAtPosition(
                this->app->mouse->position,
                this->app->camera(),
                this->selectionNodeMask
            );

        // Report successful selection.
        if (node)
        {
            this->selectedNode = node;
            this->selectedNodeChanged.report();
        }
    }
