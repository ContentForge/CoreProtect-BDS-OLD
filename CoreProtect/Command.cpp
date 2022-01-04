#include "pch.h"
#include "Command.h"

#define DEFAULT_COMMAND_FLAGS { (CommandFlagValue)0 }, { (CommandFlagValue)0x80 }


enum class CoreProtect::Option : int
{
	HELP = 1,
	INSPECT = 2,
	I = 3,
	ROLLBACK = 4,
	LOOKUP = 5,
	L = 6
} option;


Logger logger;

void CoreProtect::execute(CommandOrigin const& ori, CommandOutput& outp) const
{
	ServerPlayer* pl = ori.getPlayer();

	switch (option)
	{
	case Option::HELP:                                     // co help...
		pl->sendText(tr("coreprotect.help"));
		break;
	case Option::INSPECT:                                  // co inspect...
		if (!pl->hasTag("inspect")) pl->addTag("inspect");
		else pl->removeTag("inspect");
		break;
	case Option::I:                                        // co i...
		if (!pl->hasTag("inspect")) pl->addTag("inspect");
		else pl->removeTag("inspect");
	case Option::ROLLBACK:                                 // co rollback...
		pl->sendText("rollback?");
		break;
	case Option::LOOKUP:                                   // co lookup...
		break;
	case Option::L:                                        // co l...
		break;
	}
	/*if (option == "help")
	{
		ori.getPlayer()->sendText(tr("coreprotect.help"));
	}
	else if (option == "inspect")
	{
		std::cout << option << std::endl;
	}*/
}


enum CommandParameterOption;

void CoreProtect::setup(CommandRegistry* r)
{
	using RegisterCommandHelper::makeMandatory;
	using RegisterCommandHelper::makeOptional;

	r->registerCommand("coreprotect", "CoreProtect", CommandPermissionLevel::Any, DEFAULT_COMMAND_FLAGS); // coreprotect
	r->registerAlias("coreprotect", "co");                                                                // co (alias)

	r->addEnum<Option>("help", { {"help", Option::HELP} });                           // help
	r->addSoftEnum("helpParam", { {"help"}, {"inspect"}, {"rollback"}, {"lookup"} }); // help param
	r->addEnum<Option>("inspect", { {"inspect", Option::INSPECT} });                  // inspect
	r->addEnum<Option>("i", { {"i", Option::I} });                                    // i (inspect)
	r->addEnum<Option>("rollback", { {"rollback", Option::ROLLBACK} });               // rollback
	r->addEnum<Option>("lookup", { {"lookup", Option::LOOKUP} });                     // lookup
	r->addEnum<Option>("l", { {"l", Option::L} });                                    // l (lookup)

	auto helpOption = makeMandatory<CommandParameterDataType::ENUM>(&CoreProtect::option, "help", "help").addOptions((CommandParameterOption)1);			 // help
	auto helpParam = makeOptional<CommandParameterDataType::SOFT_ENUM>(&CoreProtect::helpParam, "param", "helpParam", &CoreProtect::isSetHelpParam);         // help param
	auto inspectOption = makeMandatory<CommandParameterDataType::ENUM>(&CoreProtect::option, "inspect", "inspect").addOptions((CommandParameterOption)1);    // inspect
	auto iOption = makeMandatory<CommandParameterDataType::ENUM>(&CoreProtect::option, "i", "i").addOptions((CommandParameterOption)1);						 // i (inspect)
	auto rollbackOption = makeMandatory<CommandParameterDataType::ENUM>(&CoreProtect::option, "rollback", "rollback").addOptions((CommandParameterOption)1); // rollback
	auto lookupOption = makeMandatory<CommandParameterDataType::ENUM>(&CoreProtect::option, "lookup", "lookup").addOptions((CommandParameterOption)1);       // lookup
	auto lOption = makeMandatory<CommandParameterDataType::ENUM>(&CoreProtect::option, "l", "l").addOptions((CommandParameterOption)1);                      // l (lookup)
	auto lPageParam = makeMandatory(&CoreProtect::lookupPage, "page");																						 // lookup page
	auto rollbackTimeParam = makeMandatory(&CoreProtect::rollbackTime, "time");																				 // rollback time
	auto rollbackActionParam = makeOptional(&CoreProtect::rollbackAction, "action", &CoreProtect::isSetRollbackAction);									     // rollback action

	r->registerOverload<CoreProtect>("co", helpOption, helpParam);                                  // co help [param: string]
	r->registerOverload<CoreProtect>("co", inspectOption);                                          // co inspect
	r->registerOverload<CoreProtect>("co", iOption);                                                // co i
	r->registerOverload<CoreProtect>("co", rollbackOption, rollbackTimeParam, rollbackActionParam); // co rollback <time: int> [action: string]
	r->registerOverload<CoreProtect>("co", lookupOption);                                           // co lookup
	r->registerOverload<CoreProtect>("co", lOption, lPageParam);                                    // co l <page: int>
}