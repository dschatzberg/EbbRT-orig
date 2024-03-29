#include <unordered_map>

#include "arch/args.hpp"
#include "lrt/boot.hpp"
#include "lrt/event.hpp"
#include "lrt/mem.hpp"
#include "lrt/trans_impl.hpp"

namespace {
  class RootBinding {
  public:
    ebbrt::lrt::trans::EbbId id;
    ebbrt::lrt::trans::EbbRoot* root;
  };
  RootBinding* initial_root_table;
  ebbrt::lrt::trans::EbbRoot* miss_handler;
}

ebbrt::lrt::trans::LocalEntry** ebbrt::lrt::trans::phys_local_entries;
ebbrt::lrt::trans::LocalEntry* const local_table =
  reinterpret_cast<ebbrt::lrt::trans::LocalEntry*>
  (ebbrt::lrt::trans::LOCAL_MEM_VIRT);

ebbrt::lrt::trans::InitRoot ebbrt::lrt::trans::init_root;

void
ebbrt::lrt::trans::init_ebbs()
{
  miss_handler = &init_root;
  const boot::Config* config = boot::get_config();
  initial_root_table = new (mem::malloc(sizeof(RootBinding) * config->count, 0))
    RootBinding[config->count];
  auto ptr = config->table;
  for (unsigned i = 0; i < config->count; ++i) {
    void* addr = boot::find_symbol(ptr->symbol);
    auto create_root = reinterpret_cast<EbbRoot* (*)()>(addr);
    initial_root_table[i].id = ptr->id;
    initial_root_table[i].root = create_root();
    uintptr_t next = reinterpret_cast<uintptr_t>(ptr->symbol +
                                                 std::strlen(ptr->symbol));
    next = next + alignof(boot::Config::CreateRoot) -
      (next % alignof(boot::Config::CreateRoot));
    ptr = reinterpret_cast<boot::Config::CreateRoot*>(next);
  }
}

bool
ebbrt::lrt::trans::init(unsigned num_cores)
{
  return true;
}

void
ebbrt::lrt::trans::init_cpu()
{
  init_cpu_arch();
  for (auto i = 0; i < NUM_LOCAL_ENTRIES; ++i) {
    local_table[i].ref = reinterpret_cast<EbbRep*>
      (&local_table[i].rep);
    local_table[i].rep = &default_vtable;
  }
}

bool
ebbrt::lrt::trans::_trans_precall(ebbrt::Args* args,
                                  ptrdiff_t fnum,
                                  FuncRet* fret)
{
  void* rep = *reinterpret_cast<void**>(args);
  LocalEntry* le =
    reinterpret_cast<LocalEntry*>
    (static_cast<uintptr_t*>(rep) - 1);
  uintptr_t loc = reinterpret_cast<uintptr_t>(le);
  EbbId id =
    (loc - reinterpret_cast<uintptr_t>(LOCAL_MEM_VIRT)) /
    sizeof(LocalEntry);
  return miss_handler->PreCall(args, fnum, fret, id);
}

void*
ebbrt::lrt::trans::_trans_postcall(void* ret)
{
  return miss_handler->PostCall(ret);
}

bool
ebbrt::lrt::trans::InitRoot::PreCall(ebbrt::Args* args,
                                     ptrdiff_t fnum,
                                     FuncRet* fret,
                                     EbbId id)
{
  const boot::Config* config = boot::get_config();
  EbbRoot* root = nullptr;
  for (unsigned i = 0; i < config->count; ++i) {
    if (initial_root_table[i].id == id) {
      root = initial_root_table[i].root;
      break;
    }
  }
  if (root == nullptr) {
    while (1)
      ;
  }
  bool ret = root->PreCall(args, fnum, fret, id);
  if (ret) {
    event::_event_altstack_push
      (reinterpret_cast<uintptr_t>(root));
  }
  return ret;
}

void*
ebbrt::lrt::trans::InitRoot::PostCall(void* ret)
{
  EbbRoot* root =
    reinterpret_cast<EbbRoot*>
    (event::_event_altstack_pop());
  return root->PostCall(ret);
}

void
ebbrt::lrt::trans::cache_rep(EbbId id, EbbRep* rep)
{
  reinterpret_cast<LocalEntry*>(LOCAL_MEM_VIRT)[id].ref = rep;
}


extern "C" void default_func0();
extern "C" void default_func1();
extern "C" void default_func2();
extern "C" void default_func3();
extern "C" void default_func4();
extern "C" void default_func5();
extern "C" void default_func6();
extern "C" void default_func7();
extern "C" void default_func8();
extern "C" void default_func9();

extern "C" void default_func10();
extern "C" void default_func11();
extern "C" void default_func12();
extern "C" void default_func13();
extern "C" void default_func14();
extern "C" void default_func15();
extern "C" void default_func16();
extern "C" void default_func17();
extern "C" void default_func18();
extern "C" void default_func19();

extern "C" void default_func20();
extern "C" void default_func21();
extern "C" void default_func22();
extern "C" void default_func23();
extern "C" void default_func24();
extern "C" void default_func25();
extern "C" void default_func26();
extern "C" void default_func27();
extern "C" void default_func28();
extern "C" void default_func29();

extern "C" void default_func30();
extern "C" void default_func31();
extern "C" void default_func32();
extern "C" void default_func33();
extern "C" void default_func34();
extern "C" void default_func35();
extern "C" void default_func36();
extern "C" void default_func37();
extern "C" void default_func38();
extern "C" void default_func39();

extern "C" void default_func40();
extern "C" void default_func41();
extern "C" void default_func42();
extern "C" void default_func43();
extern "C" void default_func44();
extern "C" void default_func45();
extern "C" void default_func46();
extern "C" void default_func47();
extern "C" void default_func48();
extern "C" void default_func49();

extern "C" void default_func50();
extern "C" void default_func51();
extern "C" void default_func52();
extern "C" void default_func53();
extern "C" void default_func54();
extern "C" void default_func55();
extern "C" void default_func56();
extern "C" void default_func57();
extern "C" void default_func58();
extern "C" void default_func59();

extern "C" void default_func60();
extern "C" void default_func61();
extern "C" void default_func62();
extern "C" void default_func63();
extern "C" void default_func64();
extern "C" void default_func65();
extern "C" void default_func66();
extern "C" void default_func67();
extern "C" void default_func68();
extern "C" void default_func69();

extern "C" void default_func70();
extern "C" void default_func71();
extern "C" void default_func72();
extern "C" void default_func73();
extern "C" void default_func74();
extern "C" void default_func75();
extern "C" void default_func76();
extern "C" void default_func77();
extern "C" void default_func78();
extern "C" void default_func79();

extern "C" void default_func80();
extern "C" void default_func81();
extern "C" void default_func82();
extern "C" void default_func83();
extern "C" void default_func84();
extern "C" void default_func85();
extern "C" void default_func86();
extern "C" void default_func87();
extern "C" void default_func88();
extern "C" void default_func89();

extern "C" void default_func90();
extern "C" void default_func91();
extern "C" void default_func92();
extern "C" void default_func93();
extern "C" void default_func94();
extern "C" void default_func95();
extern "C" void default_func96();
extern "C" void default_func97();
extern "C" void default_func98();
extern "C" void default_func99();

extern "C" void default_func100();
extern "C" void default_func101();
extern "C" void default_func102();
extern "C" void default_func103();
extern "C" void default_func104();
extern "C" void default_func105();
extern "C" void default_func106();
extern "C" void default_func107();
extern "C" void default_func108();
extern "C" void default_func109();

extern "C" void default_func110();
extern "C" void default_func111();
extern "C" void default_func112();
extern "C" void default_func113();
extern "C" void default_func114();
extern "C" void default_func115();
extern "C" void default_func116();
extern "C" void default_func117();
extern "C" void default_func118();
extern "C" void default_func119();

extern "C" void default_func120();
extern "C" void default_func121();
extern "C" void default_func122();
extern "C" void default_func123();
extern "C" void default_func124();
extern "C" void default_func125();
extern "C" void default_func126();
extern "C" void default_func127();
extern "C" void default_func128();
extern "C" void default_func129();

extern "C" void default_func130();
extern "C" void default_func131();
extern "C" void default_func132();
extern "C" void default_func133();
extern "C" void default_func134();
extern "C" void default_func135();
extern "C" void default_func136();
extern "C" void default_func137();
extern "C" void default_func138();
extern "C" void default_func139();

extern "C" void default_func140();
extern "C" void default_func141();
extern "C" void default_func142();
extern "C" void default_func143();
extern "C" void default_func144();
extern "C" void default_func145();
extern "C" void default_func146();
extern "C" void default_func147();
extern "C" void default_func148();
extern "C" void default_func149();

extern "C" void default_func150();
extern "C" void default_func151();
extern "C" void default_func152();
extern "C" void default_func153();
extern "C" void default_func154();
extern "C" void default_func155();
extern "C" void default_func156();
extern "C" void default_func157();
extern "C" void default_func158();
extern "C" void default_func159();

extern "C" void default_func160();
extern "C" void default_func161();
extern "C" void default_func162();
extern "C" void default_func163();
extern "C" void default_func164();
extern "C" void default_func165();
extern "C" void default_func166();
extern "C" void default_func167();
extern "C" void default_func168();
extern "C" void default_func169();

extern "C" void default_func170();
extern "C" void default_func171();
extern "C" void default_func172();
extern "C" void default_func173();
extern "C" void default_func174();
extern "C" void default_func175();
extern "C" void default_func176();
extern "C" void default_func177();
extern "C" void default_func178();
extern "C" void default_func179();

extern "C" void default_func180();
extern "C" void default_func181();
extern "C" void default_func182();
extern "C" void default_func183();
extern "C" void default_func184();
extern "C" void default_func185();
extern "C" void default_func186();
extern "C" void default_func187();
extern "C" void default_func188();
extern "C" void default_func189();

extern "C" void default_func190();
extern "C" void default_func191();
extern "C" void default_func192();
extern "C" void default_func193();
extern "C" void default_func194();
extern "C" void default_func195();
extern "C" void default_func196();
extern "C" void default_func197();
extern "C" void default_func198();
extern "C" void default_func199();

extern "C" void default_func200();
extern "C" void default_func201();
extern "C" void default_func202();
extern "C" void default_func203();
extern "C" void default_func204();
extern "C" void default_func205();
extern "C" void default_func206();
extern "C" void default_func207();
extern "C" void default_func208();
extern "C" void default_func209();

extern "C" void default_func210();
extern "C" void default_func211();
extern "C" void default_func212();
extern "C" void default_func213();
extern "C" void default_func214();
extern "C" void default_func215();
extern "C" void default_func216();
extern "C" void default_func217();
extern "C" void default_func218();
extern "C" void default_func219();

extern "C" void default_func220();
extern "C" void default_func221();
extern "C" void default_func222();
extern "C" void default_func223();
extern "C" void default_func224();
extern "C" void default_func225();
extern "C" void default_func226();
extern "C" void default_func227();
extern "C" void default_func228();
extern "C" void default_func229();

extern "C" void default_func230();
extern "C" void default_func231();
extern "C" void default_func232();
extern "C" void default_func233();
extern "C" void default_func234();
extern "C" void default_func235();
extern "C" void default_func236();
extern "C" void default_func237();
extern "C" void default_func238();
extern "C" void default_func239();

extern "C" void default_func240();
extern "C" void default_func241();
extern "C" void default_func242();
extern "C" void default_func243();
extern "C" void default_func244();
extern "C" void default_func245();
extern "C" void default_func246();
extern "C" void default_func247();
extern "C" void default_func248();
extern "C" void default_func249();

extern "C" void default_func250();
extern "C" void default_func251();
extern "C" void default_func252();
extern "C" void default_func253();
extern "C" void default_func254();
extern "C" void default_func255();

void (*ebbrt::lrt::trans::default_vtable[256])() = {
  default_func0,
  default_func1,
  default_func2,
  default_func3,
  default_func4,
  default_func5,
  default_func6,
  default_func7,
  default_func8,
  default_func9,
  default_func10,
  default_func11,
  default_func12,
  default_func13,
  default_func14,
  default_func15,
  default_func16,
  default_func17,
  default_func18,
  default_func19,
  default_func20,
  default_func21,
  default_func22,
  default_func23,
  default_func24,
  default_func25,
  default_func26,
  default_func27,
  default_func28,
  default_func29,
  default_func30,
  default_func31,
  default_func32,
  default_func33,
  default_func34,
  default_func35,
  default_func36,
  default_func37,
  default_func38,
  default_func39,
  default_func40,
  default_func41,
  default_func42,
  default_func43,
  default_func44,
  default_func45,
  default_func46,
  default_func47,
  default_func48,
  default_func49,
  default_func50,
  default_func51,
  default_func52,
  default_func53,
  default_func54,
  default_func55,
  default_func56,
  default_func57,
  default_func58,
  default_func59,
  default_func60,
  default_func61,
  default_func62,
  default_func63,
  default_func64,
  default_func65,
  default_func66,
  default_func67,
  default_func68,
  default_func69,
  default_func70,
  default_func71,
  default_func72,
  default_func73,
  default_func74,
  default_func75,
  default_func76,
  default_func77,
  default_func78,
  default_func79,
  default_func80,
  default_func81,
  default_func82,
  default_func83,
  default_func84,
  default_func85,
  default_func86,
  default_func87,
  default_func88,
  default_func89,
  default_func90,
  default_func91,
  default_func92,
  default_func93,
  default_func94,
  default_func95,
  default_func96,
  default_func97,
  default_func98,
  default_func99,
  default_func100,
  default_func101,
  default_func102,
  default_func103,
  default_func104,
  default_func105,
  default_func106,
  default_func107,
  default_func108,
  default_func109,
  default_func110,
  default_func111,
  default_func112,
  default_func113,
  default_func114,
  default_func115,
  default_func116,
  default_func117,
  default_func118,
  default_func119,
  default_func120,
  default_func121,
  default_func122,
  default_func123,
  default_func124,
  default_func125,
  default_func126,
  default_func127,
  default_func128,
  default_func129,
  default_func130,
  default_func131,
  default_func132,
  default_func133,
  default_func134,
  default_func135,
  default_func136,
  default_func137,
  default_func138,
  default_func139,
  default_func140,
  default_func141,
  default_func142,
  default_func143,
  default_func144,
  default_func145,
  default_func146,
  default_func147,
  default_func148,
  default_func149,
  default_func150,
  default_func151,
  default_func152,
  default_func153,
  default_func154,
  default_func155,
  default_func156,
  default_func157,
  default_func158,
  default_func159,
  default_func160,
  default_func161,
  default_func162,
  default_func163,
  default_func164,
  default_func165,
  default_func166,
  default_func167,
  default_func168,
  default_func169,
  default_func170,
  default_func171,
  default_func172,
  default_func173,
  default_func174,
  default_func175,
  default_func176,
  default_func177,
  default_func178,
  default_func179,
  default_func180,
  default_func181,
  default_func182,
  default_func183,
  default_func184,
  default_func185,
  default_func186,
  default_func187,
  default_func188,
  default_func189,
  default_func190,
  default_func191,
  default_func192,
  default_func193,
  default_func194,
  default_func195,
  default_func196,
  default_func197,
  default_func198,
  default_func199,
  default_func200,
  default_func201,
  default_func202,
  default_func203,
  default_func204,
  default_func205,
  default_func206,
  default_func207,
  default_func208,
  default_func209,
  default_func210,
  default_func211,
  default_func212,
  default_func213,
  default_func214,
  default_func215,
  default_func216,
  default_func217,
  default_func218,
  default_func219,
  default_func220,
  default_func221,
  default_func222,
  default_func223,
  default_func224,
  default_func225,
  default_func226,
  default_func227,
  default_func228,
  default_func229,
  default_func230,
  default_func231,
  default_func232,
  default_func233,
  default_func234,
  default_func235,
  default_func236,
  default_func237,
  default_func238,
  default_func239,
  default_func240,
  default_func241,
  default_func242,
  default_func243,
  default_func244,
  default_func245,
  default_func246,
  default_func247,
  default_func248,
  default_func249,
  default_func250,
  default_func251,
  default_func252,
  default_func253,
  default_func254,
  default_func255
};
