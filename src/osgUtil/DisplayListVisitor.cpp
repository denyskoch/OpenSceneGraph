#include <osgUtil/DisplayListVisitor>
#include <osg/Drawable>

using namespace osg;
using namespace osgUtil;

DisplayListVisitor::DisplayListVisitor(Mode mode)
{
    setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
    
    _mode = mode;
    
    _state = NULL;
}


void DisplayListVisitor::apply(osg::Node& node)
{
    if ((_mode&COMPILE_STATE_ATTRIBUTES) && node.getStateSet() && _state.valid())
    {
        node.getStateSet()->compile(*_state);
    }

    traverse(node);
}

void DisplayListVisitor::apply(osg::Geode& node)
{
    if (_mode&COMPILE_STATE_ATTRIBUTES && _state.valid())
    {
        if (node.getStateSet())
        {
            node.getStateSet()->compile(*_state);
        }

        for(int i=0;i<node.getNumDrawables();++i)
        {
            Drawable* drawable = node.getDrawable(i);
            if (drawable->getUseDisplayList())
            {
                if (drawable->getStateSet())
                {
                    drawable->getStateSet()->compile(*_state);
                }
            }
        }
    }

    if (_mode&SWITCH_OFF_DISPLAY_LISTS)
    {
        for(int i=0;i<node.getNumDrawables();++i)
        {
            node.getDrawable(i)->setUseDisplayList(false);
        }
    }
    if (_mode&SWITCH_ON_DISPLAY_LISTS)
    {
        for(int i=0;i<node.getNumDrawables();++i)
        {
            node.getDrawable(i)->setUseDisplayList(true);
        }
    }

    if (_mode&COMPILE_DISPLAY_LISTS && _state.valid())
    {
        for(int i=0;i<node.getNumDrawables();++i)
        {
            if (node.getDrawable(i)->getUseDisplayList())
            {
                node.getDrawable(i)->compile(*_state);
            }
        }
    }
}
