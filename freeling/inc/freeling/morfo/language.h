//////////////////////////////////////////////////////////////////
//
//    FreeLing - Open Source Language Analyzers
//
//    Copyright (C) 2004   TALP Research Center
//                         Universitat Politecnica de Catalunya
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU General Public
//    License as published by the Free Software Foundation; either
//    version 3 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy of the GNU General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//    contact: Lluis Padro (padro@lsi.upc.es)
//             TALP Research Center
//             despatx C6.212 - Campus Nord UPC
//             08034 Barcelona.  SPAIN
//
////////////////////////////////////////////////////////////////

#ifndef _LANGUAGE
#define _LANGUAGE

#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>

#include "freeling/regexp.h"
#include "freeling/windll.h"
#include "freeling/tree.h"

namespace freeling {

  class word; // predeclaration

  ////////////////////////////////////////////////////////////////
  ///   Class analysis stores a possible reading (lemma, PoS, probability, distance)
  ///   for a word.
  ////////////////////////////////////////////////////////////////

  class WINDLL analysis {

  private:
    /// lemma
    std::wstring lemma;
    /// PoS tag
    std::wstring tag;
    /// probability of that lemma-tag given the word
    double prob;
    /// distance from a added analysis from corrector to the original word
    double distance;
    ///  list of possible senses for that analysis, along with their rank as attributed by the ukb PPR algorithm
    std::list<std::pair<std::wstring,double> > senses;
    /// information to retokenize the word after tagging if this analysis is selected
    std::list<word> retok;

    // store which sequences --among the kbest proposed by 
    // the tagger-- contain this analysis
    std::set<int> selected_kbest;

  public:
    /// user-managed data, we just store it.
    std::vector<std::wstring> user;

    /// constructor
    analysis();
    /// constructor
    analysis(const std::wstring &, const std::wstring &);
    /// assignment
    analysis& operator=(const analysis&);

    void init(const std::wstring &l, const std::wstring &t);
    void set_lemma(const std::wstring &);
    void set_tag(const std::wstring &);
    void set_prob(double);
    void set_distance(double);
    void set_retokenizable(const std::list<word> &);

    bool has_prob() const;
    bool has_distance() const;
    std::wstring get_lemma() const;
    std::wstring get_tag() const;
    double get_prob() const;
    double get_distance() const;
    bool is_retokenizable() const;
    std::list<word>& get_retokenizable();
    const std::list<word>& get_retokenizable() const;

    const std::list<std::pair<std::wstring,double> > & get_senses() const;
    std::list<std::pair<std::wstring,double> > & get_senses();
    void set_senses(const std::list<std::pair<std::wstring,double> > &);
    // useful for java API
    std::wstring get_senses_string() const;

    // get the largest kbest sequence index the analysis is selected in.
    int max_kbest() const;
    // find out whether the analysis is selected in the tagger k-th best sequence
    bool is_selected(int k=0) const;
    // mark this analysis as selected in k-th best sequence
    void mark_selected(int k=0);
    // unmark this analysis as selected in k-th best sequence
    void unmark_selected(int k=0);

    /// Comparison to sort analysis by *decreasing* probability
    bool operator>(const analysis &) const;
    /// Comparison to sort analysis by *increasing* probability
    bool operator<(const analysis &) const;
    /// Comparison (to please MSVC)
    bool operator==(const analysis &) const;
  };


  ////////////////////////////////////////////////////////////////
  ///   Class word stores all info related to a word:
  ///  form, list of analysis, list of tokens (if multiword).
  ////////////////////////////////////////////////////////////////

  class WINDLL word : public std::list<analysis> {
  private:
    /// lexical form
    std::wstring form;
    /// lexical form, lowercased
    std::wstring lc_form;
    /// phonetic form
    std::wstring ph_form;
    /// empty list if not a multiword
    std::list<word> multiword;
    /// whether the multiword presents segmentantion ambiguity (i.e. could not be a mw)
    bool ambiguous_mw;
    /// alternative forms provided by orthographic or phonetic SED
    std::list<std::pair<std::wstring,int> > alternatives;
    /// token span
    unsigned long start, finish;
    /// word form found in dictionary
    bool in_dict;
    /// word morphological shouldn't be further modified
    bool locked;
    /// clone word (used by assignment/copy constructors)
    void clone(const word &);
    /// position of word in the sentence (count from 0)
    size_t position;
 
    /// Values for word::iterator types
    static const int SELECTED=0;
    static const int UNSELECTED=1;
    static const int ALL=2;

  public:
    // predeclarations
    class iterator; 
    class const_iterator; 

    /// user-managed data, we just store it.
    std::vector<std::wstring> user;

    /// constructor
    word();
    /// constructor
    word(const std::wstring &);
    /// constructor
    word(const std::wstring &, const std::list<word> &);
    /// constructor
    word(const std::wstring &, const std::list<analysis> &, const std::list<word> &);
    /// Copy constructor
    word(const word &);
    /// assignment
    word& operator=(const word&);

    /// copy analysis from another word
    void copy_analysis(const word &);
    /// Get the number of selected analysis
    int get_n_selected(int k=0) const;
    /// get the number of unselected analysis
    int get_n_unselected(int k=0) const;
    /// true iff the word is a multiword compound
    bool is_multiword() const;
    /// true iff the word is a multiword marked as ambiguous
    bool is_ambiguous_mw() const;
    /// set mw ambiguity status
    void set_ambiguous_mw(bool);
    /// get number of words in compound
    int get_n_words_mw() const;
    /// get word objects that compound the multiword
    const std::list<word>& get_words_mw() const;
    /// get word form
    std::wstring get_form() const;
    /// Get word form, lowercased.
    std::wstring get_lc_form() const;
    /// Get word phonetic form
    std::wstring get_ph_form() const;
    /// Get an iterator to the first selected analysis
    word::iterator selected_begin(int k=0);
    /// Get an iterator to the first selected analysis
    word::const_iterator selected_begin(int k=0) const;
    /// Get an iterator to the end of selected analysis list
    word::iterator selected_end(int k=0);
    /// Get an iterator to the end of selected analysis list
    word::const_iterator selected_end(int k=0) const;
    /// Get an iterator to the first unselected analysis
    word::iterator unselected_begin(int k=0);
    /// Get an iterator to the first unselected analysis
    word::const_iterator unselected_begin(int k=0) const;
    /// Get an iterator to the end of unselected analysis list
    word::iterator unselected_end(int k=0);
    /// Get an iterator to the end of unselected analysis list
    word::const_iterator unselected_end(int k=0) const;
    /// Get how many kbest tags the word has
    unsigned int num_kbest() const;
    /// get lemma for the selected analysis in list
    std::wstring get_lemma(int k=0) const;
    /// get tag for the selected analysis
    std::wstring get_tag(int k=0) const;
    /// get tag (short version) for the selected analysis, assuming eagles tagset
    std::wstring get_short_tag(int k=0) const;
    /// get tag (short version) for the selected analysis
    std::wstring get_short_tag(const std::wstring &,int k=0) const;

    /// get sense list for the selected analysis
    const std::list<std::pair<std::wstring,double> >& get_senses(int k=0) const;
    std::list<std::pair<std::wstring,double> >& get_senses(int k=0);
    // useful for java API
    std::wstring get_senses_string(int k=0) const;
    /// set sense list for the selected analysis
    void set_senses(const std::list<std::pair<std::wstring,double> > &, int k=0);

    /// get token span.
    unsigned long get_span_start() const;
    unsigned long get_span_finish() const;

    /// get in_dict
    bool found_in_dict() const;
    /// set in_dict
    void set_found_in_dict(bool);
    /// check if there is any retokenizable analysis
    bool has_retokenizable() const;
    /// mark word as having definitive analysis
    void lock_analysis();
    /// check if word is marked as having definitive analysis
    bool is_locked() const;

    /// add an alternative to the alternatives list
    void add_alternative(const std::wstring &, int);
    /// replace alternatives list with list given
    void set_alternatives(const std::list<std::pair<std::wstring,int> > &);
    /// clear alternatives list
    void clear_alternatives();
    /// find out if the speller checked alternatives
    bool has_alternatives() const;
    /// get alternatives list const &
    const std::list<std::pair<std::wstring,int> >& get_alternatives() const;
    /// get alternatives list &
    std::list<std::pair<std::wstring,int> >& get_alternatives();
    /// get alternatives begin iterator
    std::list<std::pair<std::wstring,int> >::iterator alternatives_begin();
    /// get alternatives end iterator
    std::list<std::pair<std::wstring,int> >::iterator alternatives_end();
    /// get alternatives begin iterator
    std::list<std::pair<std::wstring,int> >::const_iterator alternatives_begin() const;
    /// get alternatives end iterator
    std::list<std::pair<std::wstring,int> >::const_iterator alternatives_end() const;

    /// add one analysis to current analysis list  (no duplicate check!)
    void add_analysis(const analysis &);
    /// set analysis list to one single analysis, overwriting current values
    void set_analysis(const analysis &);
    /// set analysis list, overwriting current values
    void set_analysis(const std::list<analysis> &);
    /// set word form
    void set_form(const std::wstring &);
    /// Set word phonetic form
    void set_ph_form(const std::wstring &);
    /// set token span
    void set_span(unsigned long, unsigned long);

    // get/set word position
    void set_position(size_t);
    size_t get_position() const;

    /// look for an analysis with a tag matching given regexp
    bool find_tag_match(const freeling::regexp &) const;

    /// get number of analysis in current list
    int get_n_analysis() const;
    /// empty the list of selected analysis
    void unselect_all_analysis(int k=0);
    /// mark all analysisi as selected
    void select_all_analysis(int k=0);
    /// add the given analysis to selected list.
    void select_analysis(word::iterator, int k=0);
    /// remove the given analysis from selected list.
    void unselect_analysis(word::iterator, int k=0);
    /// get list of analysis (useful for perl API)
    std::list<analysis> get_analysis() const;
    /// get begin iterator to analysis list (useful for perl/java API)
    word::iterator analysis_begin();
    word::const_iterator analysis_begin() const;
    /// get end iterator to analysis list (useful for perl/java API)
    word::iterator analysis_end();
    word::const_iterator analysis_end() const;

    /// iterator over word analysis (either all, only selected, only unselected)
    class WINDLL iterator : public std::list<analysis>::iterator {
      friend class word::const_iterator;
    private:
      /// Iterator range begin
      std::list<analysis>::iterator ibeg;
      /// Iterator range end
      std::list<analysis>::iterator iend;
      /// Iterator type (ALL,SELECTED,UNSELECTED)
      int type;
      /// Which of k-best sequences proposed by the tagger is the iterator associated with.
      int kbest;

    public:
      /// empty constructor
      iterator();
      /// copy
      iterator(const word::iterator &);
      /// Constructor from std::list iterator
      iterator(const std::list<analysis>::iterator &);
      /// Constructor for filtered iterators (selected/unselected)
      iterator(const std::list<analysis>::iterator &, 
               const std::list<analysis>::iterator &, 
               const std::list<analysis>::iterator &, int,int k=0);  
      /// Generic increment, for all cases
      iterator& operator++();
      iterator operator++(int);
    };
  
    /// const_iterator over word analysis (either all, only selected, only unselected)
    class WINDLL const_iterator : public std::list<analysis>::const_iterator {
    private:
      /// Iterator range begin
      std::list<analysis>::const_iterator ibeg;
      /// Iterator range end
      std::list<analysis>::const_iterator iend;
      /// Iterator type (ALL,SELECTED,UNSELECTED)
      int type;
      /// Which of k-best sequences proposed by the tagger is the iterator associated with.
      int kbest;

    public:
      /// empty constructor
      const_iterator();
      /// copy
      const_iterator(const word::const_iterator &);
      /// copy from nonconst iterator
      const_iterator(const word::iterator &);
      /// Constructor from std::list iterator
      const_iterator(const std::list<analysis>::const_iterator &);
      /// Constructor from std::list nonconst iterator
      const_iterator(const std::list<analysis>::iterator &);
      /// Constructor for filtered iterators (selected/unselected)
      const_iterator(const std::list<analysis>::const_iterator &,
                     const std::list<analysis>::const_iterator &, 
                     const std::list<analysis>::const_iterator &, int, int k=0);
      /// Generic increment, for all cases
      const_iterator& operator++();  
      const_iterator operator++(int);  
    };

  };



  ////////////////////////////////////////////////////////////////
  ///   Class node stores nodes of a parse_tree
  ///  Each node in the tree is either a label (intermediate node)
  ///  or a word (leaf node)
  ////////////////////////////////////////////////////////////////

  class WINDLL node {
  protected:
    /// node identifier
    std::wstring nodeid;
    /// is the node the head of the rule?
    bool head;
    /// is the node the root of a chunk? which?
    int chunk;
    /// node label
    std::wstring label;
    /// sentence word related to the node (if leaf)
    word * w;

  public:
    /// user-managed data, we just store it.
    std::vector<std::wstring> user;

    /// constructors
    node();
    node(const std::wstring &);
    /// get node identifier
    std::wstring get_node_id() const;
    /// set node identifier
    void set_node_id(const std::wstring &);
    /// get node label
    std::wstring get_label() const;
    /// get node word
    const word & get_word() const;
    /// get reference to node word
    word & get_word();
    /// set node label
    void set_label(const std::wstring &);
    /// set node word
    void set_word(word &);
    /// find out whether node is a head
    bool is_head() const;
    /// set whether node is a head
    void set_head(const bool);
    /// find out whether node is a chunk
    bool is_chunk() const;
    /// set position of the chunk in the sentence
    void set_chunk(const int);
    /// get position of the chunk in the sentence
    int  get_chunk_ord() const;

  };

  ////////////////////////////////////////////////////////////////
  ///   Class parse tree is used to store the results of parsing
  ////////////////////////////////////////////////////////////////

  class WINDLL parse_tree : public tree<node> {
  private:
    // access nodes by id
    std::map<std::wstring,parse_tree::iterator> node_index;
    // acces leaf nodes by word position
    std::vector<parse_tree::iterator> word_index;

  public:
    parse_tree();
    parse_tree(parse_tree::iterator p);
    parse_tree(const node &);

    /// assign an id to each node and build index
    void build_node_index(const std::wstring &);
    /// rebuild index maintaining node id's
    void rebuild_node_index();
    /// access the node with given id
    parse_tree::const_iterator get_node_by_id(const std::wstring &) const;
    /// access the node by word position
    parse_tree::const_iterator get_node_by_pos(size_t) const;
    /// access the node with given id
    parse_tree::iterator get_node_by_id(const std::wstring &);
    /// access the node by word position
    parse_tree::iterator get_node_by_pos(size_t);

  };


  ////////////////////////////////////////////////////////////////
  /// class denode stores nodes of a dependency tree and
  ///  parse tree <-> deptree relations
  ////////////////////////////////////////////////////////////////

  class WINDLL depnode : public node {

  private:
    /// corresponding node of the parse tree in the same sentence.
    parse_tree::iterator link;

  public:
    depnode();
    depnode(const std::wstring &);
    depnode(const node &);

    /// set link to corresponding node in the parse tree
    void set_link(const parse_tree::iterator);
    /// get link to corresponding node in the parse tree
    parse_tree::iterator get_link();
    parse_tree::const_iterator get_link() const;
    /// get link ref (useful for Java API)
    tree<node>& get_link_ref();  
  };



  ////////////////////////////////////////////////////////////////
  /// class dep_tree stores a dependency tree
  ////////////////////////////////////////////////////////////////

  class WINDLL dep_tree :  public tree<depnode> {

  private:
    // acces nodes by word position
    std::vector<dep_tree::iterator> word_index;

  public:
    dep_tree();
    dep_tree(const depnode &);

    /// get depnode corresponding to word in given position
    dep_tree::const_iterator get_node_by_pos(size_t) const;
    /// get depnode corresponding to word in given position
    dep_tree::iterator get_node_by_pos(size_t);
    /// rebuild index maintaining words positions
    void rebuild_node_index();
  };



  ////////////////////////////////////////////////////////////////
  /// Virtual class to store the processing state of a sentence.
  /// Each processor will define a derived class with needed contents,
  /// and store it in the sentence being processed.
  ////////////////////////////////////////////////////////////////

  class processor_status {
  public:
    processor_status();
    virtual ~processor_status() {};
  };


  ////////////////////////////////////////////////////////////////
  ///   Class sentence is just a list of words that someone
  /// (the splitter) has validated it as a complete sentence.
  /// It may include a parse tree.
  ////////////////////////////////////////////////////////////////

  class WINDLL sentence : public std::list<word> {
  private:
    // sentence identifier, in case user application wants to set it.
    std::wstring sent_id;
    // vector with pointers to sentence words, for fast access by position
    std::vector<word*> wpos; 
    // parse tree (if sentence parsed)
    std::map<int,parse_tree> pts;
    // dependencey tree (if sentence dep. parsed)
    std::map<int,dep_tree> dts;
    // clone sentence (used by assignment/copy constructors)
    void clone(const sentence &);
    // stack processing status for processor currently analyzing the sentence
    // (there might be a hierarchy of embeeded processors, thus the stack)
    std::list<processor_status*> status;

  public:
    typedef std::pair<std::wstring, std::map<int,std::wstring> > pred_arg_set;
    std::map<int,pred_arg_set> pred_args;

    sentence();
    sentence(const std::list<word>&);
    /// Copy constructor
    sentence(const sentence &);
    /// assignment
    sentence& operator=(const sentence&);
    /// positional access to a word
    const word& operator[](size_t) const;
    word& operator[](size_t);
    /// find out how many kbest sequences the tagger computed
    unsigned int num_kbest() const;
    /// add a word to the sentence
    void push_back(const word &);
    /// rebuild word positional index
    void rebuild_word_index();
 
    void clear();

    void set_sentence_id(const std::wstring &);
    std::wstring get_sentence_id();

    void set_parse_tree(const parse_tree &, int k=0);
    parse_tree & get_parse_tree(int k=0);
    const parse_tree & get_parse_tree(int k=0) const;
    bool is_parsed() const;

    void set_dep_tree(const dep_tree &, int k=0);
    dep_tree & get_dep_tree(int k=0);
    const dep_tree & get_dep_tree(int k=0) const;
    bool is_dep_parsed() const;

    /// get status at the top of stack
    processor_status* get_processing_status();
    const processor_status* get_processing_status() const;
    /// push status into the stack
    void set_processing_status(processor_status *);
    /// pop top status, and free it
    void clear_processing_status();

    /// get word list (useful for perl API)
    std::vector<word> get_words() const;
    /// get iterators to word list (useful for perl/java API)
    sentence::iterator words_begin();
    sentence::const_iterator words_begin() const;
    sentence::iterator words_end();
    sentence::const_iterator words_end() const;
  };

  ////////////////////////////////////////////////////////////////
  ///   Class paragraph is just a list of sentences that someone
  ///  has validated it as a paragraph.
  ////////////////////////////////////////////////////////////////

  class WINDLL paragraph : public std::list<sentence> {
  public:
    paragraph() {}
    virtual ~paragraph() {}
  };

  ////////////////////////////////////////////////////////////////
  ///   Class document is a list of paragraphs. It may have additional
  ///  information (such as title)
  ////////////////////////////////////////////////////////////////

  class WINDLL document : public std::list<paragraph> {

  private:
    paragraph title;
    std::multimap<int,std::wstring> group2node;
    std::map<std::wstring,int> node2group;
    int last_group;

  public:
    document();
    /// Ads one node to a coreference group
    void add_positive(const std::wstring &node, int group);
    /// Adds node2 to the group of node1
    void add_positive(const std::wstring &node1, const std::wstring &node2);
    /// Gets the id of the coreference group of the node
    int get_coref_group(const std::wstring&) const;
    /// Gets all the nodes in a coreference group id
    std::list<std::wstring> get_coref_nodes(int) const;
    /// Returns if two nodes are in the same coreference group
    bool is_coref(const std::wstring &, const std::wstring &) const;
  };

} // namespace

#endif

