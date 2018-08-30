FEATURE main.h/Setup
this->setupSelection();

FEATURE main.h/TearDown
this->tearSelectionDown();

FEATURE main.h/Impl
private:
    const std::string selectionCallbackName = "Selection";
    const unsigned int selectionNodeMask = 0x00000004;
    osg::Node *selectedNode = 0;
    core::Reporter selectedNodeChanged;

    void setupSelection()
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
                bool clicked = !this->app->mouse->pressedButtons.empty();
                if (clicked)
                {
                    this->selectNode();
                }
            },
            this->selectionCallbackName
        );
    }
    void tearSelectionDown()
    {
        this->app->mouse->pressedButtonsChanged.removeCallback(
            this->selectionCallbackName
        );
    }
    void selectNode()
    {
        this->selectedNode =
            scene::nodeAtPosition(
                this->app->mouse->position,
                this->app->camera(),
                this->selectionNodeMask
            );

        // Report successful selection.
        if (this->selectedNode)
        {
            this->selectedNodeChanged.report();
        }
    }
