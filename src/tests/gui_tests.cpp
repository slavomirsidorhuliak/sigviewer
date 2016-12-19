// Copyright 2013 Clemens Brunner, Thomas Brunner, Christoph Eibel,
// Alois Schlögl, Oliver Terbu.

// This file is part of SigViewer.
//
// SigViewer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SigViewer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SigViewer. If not, see <http://www.gnu.org/licenses/>.


#include "gui_tests.h"
#include "gui_impl/commands/open_file_gui_command.h"
#include "gui/application_context.h"

namespace sigviewer
{
namespace Tests_
{

//-----------------------------------------------------------------------------
void GuiTests::init ()
{
    animations_triggered_ = false;
    if (action("Animations")->isChecked())
    {
        action("Animations")->trigger();
        animations_triggered_ = true;
    }
}

//-----------------------------------------------------------------------------
QString GuiTests::run ()
{
    RUN_SUB_TEST (testEnablednessNoOpenFile())

    OpenFileGuiCommand::openFile ("blub.sinusdummy", true);
    VERIFY (applicationContext()->getCurrentFileContext().isNull() != true, "open file")

    RUN_SUB_TEST (testZooming ())
    RUN_SUB_TEST (testEventEditing ())

    currentVisModel()->selectChannel(1);
    action("Hide Channel")->trigger();
    VERIFY (currentVisModel()->getShownChannels().count(1) == 0, "hide channel")

    action("Close")->trigger();
    VERIFY (applicationContext()->getCurrentFileContext().isNull() == true, "closed file")

    RUN_SUB_TEST (testEnablednessNoOpenFile())
    return "";
}

//-----------------------------------------------------------------------------
void GuiTests::cleanup ()
{
    if (animations_triggered_)
        action ("Animations")->trigger();
}

//-----------------------------------------------------------------------------
QString GuiTests::testZooming ()
{
    /*
    unsigned old_signal_height = currentVisModel()->getSignalHeight();
    action("Zoom In Vertical")->trigger();
    unsigned new_signal_height = currentVisModel()->getSignalHeight();
    VERIFY (new_signal_height > old_signal_height, tr("zoom in vertically, old = ") + QString::number(old_signal_height) + "; new = " + QString::number(new_signal_height));

    old_signal_height = new_signal_height;
    action("Zoom Out Vertical")->trigger();
    new_signal_height = currentVisModel()->getSignalHeight();
    VERIFY (new_signal_height < old_signal_height, tr("zoom out vertically"));

    float32 old_pixel_per_sample = currentVisModel()->getPixelPerSample();
    action("Zoom In Horizontal")->trigger();
    float32 new_pixel_per_sample = currentVisModel()->getPixelPerSample();
    VERIFY (new_pixel_per_sample > old_pixel_per_sample, "zoom in horizontally");

    old_pixel_per_sample = currentVisModel()->getPixelPerSample();
    action("Zoom Out Horizontal")->trigger();
    new_pixel_per_sample = currentVisModel()->getPixelPerSample();
    VERIFY (new_pixel_per_sample < old_pixel_per_sample, "zoom out horizontally");
*/
    return "";
}

//-----------------------------------------------------------------------------
QString GuiTests::testEventEditing ()
{
    currentVisModel()->selectEvent(1);
    action ("Delete")->trigger();
    VERIFY (currentVisModel()->getEventManager()->getEvent(1).isNull(), "delete event");

    action ("Undo")->trigger();
    VERIFY (currentVisModel()->getEventManager()->getEvent(1).isNull() == false, "undo delete event");

    applicationContext()->getCurrentFileContext()->setState(FILE_STATE_UNCHANGED);

    return "";
}

//-----------------------------------------------------------------------------
QString GuiTests::testEnablednessNoOpenFile ()
{
    QStringList enabled_on_startup;
    enabled_on_startup << "Animations"
                       << "Open..."
                       << "Set Animation Duration"
                       << "About"
                       << "Run Tests..."
                       << "Exit";
    QList<QAction*> actions = GuiActionFactory::getInstance()->getQActions();
    foreach (QAction* action, actions)
    {
        VERIFY (((action->isEnabled() && enabled_on_startup.contains(action->text())) || ((action->isEnabled() == false) && (!enabled_on_startup.contains(action->text())))), QString("Action not disabled: ").append(action->text()))
    }
    return "";
}

}
}
