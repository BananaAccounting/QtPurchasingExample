#include "pch.h"
#include "MicrosoftStoreLib.h"

#include <windows.h>

using namespace Platform;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

void helloWorld()
{
	// Create the message dialog and set its content
	MessageDialog^ msg = ref new MessageDialog("No internet connection has been found.");

	// Set the command that will be invoked by default
	msg->DefaultCommandIndex = 0;

	// Set the command to be invoked when escape is pressed
	msg->CancelCommandIndex = 1;

	// Show the message dialog
	msg->ShowAsync();
}