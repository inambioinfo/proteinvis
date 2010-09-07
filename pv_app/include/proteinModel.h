#ifndef  __PROTIEN_MODEL_H
#define  __PROTIEN_MODEL_H

#include <iostream>
#include <string>

#include <glutils.h>
#include <cpputils.h>

class protein_t;

class protein_rd_t;

class protein_grouping_t;

class alpha_complex_model_t;

class pocket_model_t;

class IModelController;

class protein_grouping_ui_model_t;

class protein_model_ui_t;

class viewer_mainwindow;

typedef glutils::renderable_t array_renderer_t;

class protein_model_t:public glutils::renderable_t
{
public:
  friend class protein_model_ui_t;
  friend class viewer_mainwindow;

  enum eRenderModels
  {
    RMDL_SPACE_FILL,
    RMDL_BALL_STICK,
    RMDL_SMALL_SPACE_FILL,
    RMDL_COUNT
  };

  enum eRenderModes
  {
    RMDE_BACKBONE,
    RMDE_FULL,
    RMDE_POCKET_ATOMS,
    RMDE_NONE,
    RMDE_COUNT
  };

  enum eSurfaceRenderMode
  {
    SRM_HIDE,
    SRM_SHOW,
    SRM_SHOW_WIREFRAME,
    SRM_COUNT
  };

  enum eAlphaComplexRenderMode
  {
    ACRM_HIDE         = 0,
    ACRM_TETRAHEDRONS = 1,
    ACRM_TRIANGLES    = 2,
    ACRM_EDGES        = 4,
    ACRM_SHOWALL      = 7,
  };

  enum ePocketRenderMode
  {
    PRM_SHOW_NONE,
    PRM_SHOW_ONE,
    PRM_SHOW_ALL,
  };


protected:

  boost::shared_ptr<protein_t>             m_protein;
  boost::shared_ptr<protein_rd_t>          m_protein_rd;
  boost::shared_ptr<protein_grouping_t>    m_protein_atoms_grouping;
  boost::shared_ptr<array_renderer_t>      m_surface_renderer;
  boost::shared_ptr<alpha_complex_model_t> m_alpha_complex_model;
  boost::shared_ptr<pocket_model_t>        m_pocket_model;

  // other state stuff
  eRenderModels                   m_render_model;
  eRenderModes                    m_render_mode;
  eSurfaceRenderMode              m_surface_render_mode;
  uint                            m_alpha_complex_render_mode;
  ePocketRenderMode               m_pocket_render_mode;

  uint                            m_pocket_alpha_num;
  uint                            m_pocket_num;

  double                 m_add_atom_radius;
  double                 m_alpha_value;

  glutils::material_properties_t  m_atombond_material;
  glutils::material_properties_t  m_surface_material;

  std::string            m_protein_name;

  void render_onelevel() const;
  void render_surface() const;

  void update_pocket_render_state();
  void update_sf_model_for_pocket();

public:
  protein_model_t ( const std::string & pf);
  virtual ~protein_model_t ();

  int render();

  std::string name() const
  {
    return m_protein_name.c_str();
  }
private:
  void load_surface(const std::string &filename);

  void load_alpha_shape(const std::string &filename);

  void load_pockets(const std::string &pocf,const std::string &tetf);
};

typedef boost::shared_ptr<protein_model_t> protein_model_ptr_t;

#include <QAbstractListModel>

class protein_grouping_ui_model_t:public QAbstractItemModel
{
  Q_OBJECT

public:

  protein_grouping_ui_model_t(boost::shared_ptr<protein_grouping_t> p, QObject *parent = 0);

  ~protein_grouping_ui_model_t();

  QVariant data(const QModelIndex &index, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  QVariant headerData
      (int section, Qt::Orientation orientation,
       int role = Qt::DisplayRole) const;

  QModelIndex index
      (int row, int column,
       const QModelIndex &) const;

  QModelIndex parent(const QModelIndex &) const;

  int rowCount(const QModelIndex & ) const;

  int columnCount(const QModelIndex &) const;

  virtual bool setData
      ( const QModelIndex & index,
        const QVariant & value,
        int role );

  void set_grouping_type(int ind);

  uint get_grouping_type();

private:
  boost::shared_ptr<protein_grouping_t> m_protein_grouping;
};

#include <QScrollArea>
#include <ui_ProteinModelFrame.h>

class protein_model_ui_t:public QScrollArea,public Ui::ProteinModelFrame
{
  Q_OBJECT;

public:
  typedef protein_grouping_ui_model_t* protein_grouping_ui_model_ptr_t;

  typedef boost::shared_ptr<protein_model_t> protein_model_ptr_t;

  protein_model_ui_t(protein_model_ptr_t pptr,QWidget * par);

  void init_ui();

private:
  protein_grouping_ui_model_ptr_t m_protein_grouping_model;
  protein_model_ptr_t             m_protein_model;

  void pocket_ui_state_updated();
  void alpha_complex_ui_state_updated();


private slots:

  void on_full_mol_rendermode_radioButton_clicked ( bool );
  void on_bb_mol_rendermode_radioButton_clicked ( bool );
  void on_none_rendermode_radioButton_clicked ( bool );
  void on_pocatoms_rendermode_radioButton_clicked ( bool );

  void on_sf_model_radiobutton_clicked ( bool );
  void on_ssf_model_radiobutton_clicked ( bool );
  void on_bs_model_radioButton_clicked ( bool );
  void on_add_radius_sf_model_doubleSpinBox_valueChanged ( double );
  void on_alpha_value_sf_model_doubleSpinBox_valueChanged ( double );

  void on_show_surface_radioButton_clicked ( bool );
  void on_show_surface_wireframe_radioButton_clicked ( bool );
  void on_hide_surface_radioButton_clicked ( bool );

  void on_pocket_groupBox_clicked ( bool );
  void on_show_all_pockets_radioButton_clicked ( bool );
  void on_pocket_alpha_num_spinBox_valueChanged ( int );
  void on_pocket_num_spinBox_valueChanged ( int );

  void on_alpha_complex_groupBox_clicked ( bool );
  void on_show_alpha_complex_tets_checkBox_clicked ( bool );
  void on_show_alpha_complex_tris_checkBox_clicked ( bool );
  void on_show_alpha_complex_edges_checkBox_clicked ( bool );

  void on_protein_grouping_comboBox_currentIndexChanged(int i);
  void on_reload_pushButton_clicked(bool);
  void on_surface_color_colorpicker_colorChanged(const QColor &);
};


#endif//__PROTIEN_MODEL_H

