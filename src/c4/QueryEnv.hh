//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   c4/QueryEnv.hh
 * \author Tim Kelley
 * \date   Fri Jun 7 08:06:53 2019
 * \brief  Functions for working with your environment
 * \note   Copyright (C) 2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#ifndef Query_Env_hh
#define Query_Env_hh

#include <cstdlib> // getenv, setenv
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility> // pair

namespace rtt_c4 {

//----------------------------------------------------------------------------//
/*!
 * \brief Get a value from the environment
 *
 * \tparam T: the type of the value, must be default constructible
 *
 * \param key: the key to which you would like the corresponding value
 * \param default_value: a default value to return if key undefined.
 * \return: {whether key was defined, corresponding value}
 *
 * \note Calls POSIX getenv() function, which is not required to be re-entrant.
 * If the key was set in the environment, get_env_val() returns the value
 * converted to type T, that was set in the environment. If the key was not set
 * in the environment, it returns the default_value that the caller
 * provides. The first argument of the pair describes whether the key was
 * defined.
 */
template <typename T>
std::pair<bool, T> get_env_val(std::string const &key, T default_value = T{}) {
  static_assert(std::is_default_constructible<T>::value,
                "get_env_val only usable with default constructible types");
  T val{default_value};
  char *s_val = getenv(key.c_str());
  bool is_defined(false);
  if (s_val) {
    std::stringstream val_str(s_val);
    val_str >> val;
    is_defined = true;
  }
  return std::make_pair(is_defined, val);
} // get_env_val

//----------------------------------------------------------------------------//
/*!
 *\brief Basic information about SLURM tasks, and whether that information
 * was available.
 *
 * \note: values are based on what was in the environment at the time this
 * class is instantiated. This class is likely non-reentrant, so use with care
 * in multithreaded environments. This class relies on SLURM setting the
 * following environment variables:
 *
 * SLURM_CPUS_PER_TASK  (the argument to -c)
 * SLURM_NTASKS         (the argument to -n)
 * SLURM_JOB_NUM_NODES  (the argument to -N)
 *
 * Draco's unit tests don't really make sure that's the case, so if SLURM
 * changes, this may break. */
class SLURM_Task_Info {
public:
  /**\brief Get SLURM_CPUS_PER_TASK */
  uint32_t get_cpus_per_task() const { return cpus_per_task_; }

  /**\brief Get SLURM_NTASKS */
  uint32_t get_ntasks() const { return ntasks_; }

  /**\brief Get SLURM_JOB_NUM_NODES */
  uint32_t get_job_num_nodes() const { return job_num_nodes_; }

  /**\brief Get SLURM_CPUS_ON_NODE */
  uint32_t get_cpus_on_node() const { return cpus_on_node_; }

  //! Return value of SLURM_NODELIST
  std::string get_nodelist() const { return nodelist_; }

  /* note: these rely on the idea that n_cpus_per_task etc are never
   * going to be in the realm of 2 billion. On the blessaed day that
   * comes to pass, Machine Overlords, rethink this (please / thank you). */

  /**\brief Was SLURM_CPUS_PER_TASK set? */
  bool is_cpus_per_task_set() const { return def_cpus_per_task_; }

  /**\brief Was SLURM_NTASKS set? */
  bool is_ntasks_set() const { return def_ntasks_; }

  /**\brief Was SLURM_JOB_NUM_NODES set? */
  bool is_job_num_nodes_set() const { return def_job_num_nodes_; }

  // ctor
  SLURM_Task_Info() {
    std::tie(def_cpus_per_task_, cpus_per_task_) =
        get_env_val<int>("SLURM_CPUS_PER_TASK", cpus_per_task_);
    std::tie(def_ntasks_, ntasks_) = get_env_val<int>("SLURM_NTASKS", ntasks_);
    std::tie(def_job_num_nodes_, job_num_nodes_) =
        get_env_val<int>("SLURM_JOB_NUM_NODES", job_num_nodes_);
    std::tie(def_cpus_on_node_, cpus_on_node_) =
        get_env_val<int>("SLURM_CPUS_ON_NODE", job_num_nodes_);
    std::tie(def_nodelist_, nodelist_) =
        get_env_val<std::string>("SLURM_NODELIST");
  }

  // state
private:
  uint32_t cpus_per_task_{0xFFFFFFF}; //!< arg to -c
  //! whether SLURM_CPUS_PER_TASK was defined
  bool def_cpus_per_task_{false};
  uint32_t ntasks_{0xFFFFFFE};        //!< arg to -n
  bool def_ntasks_{false};            //!< whether SLURM_NTASKS was defined
  uint32_t job_num_nodes_{0xFFFFFFD}; //!< arg to -N
  //! whether SLURM_JOB_NUM_NODES was defined
  bool def_job_num_nodes_{false};
  uint32_t cpus_on_node_{0xFFFFFFD}; //!< SLURM_CPUS_ON_NODE
  bool def_cpus_on_node_{false};     //!< was SLURM_CPUS_ON_NODE defined?
  std::string nodelist_{"not set"};  //!< SLURM_NODELIST
  bool def_nodelist_{false};         //!< was SLURM_NODELIST defined?

}; // SLURM_Task_Info

} // namespace rtt_c4

#endif // Query_Env_hh

//---------------------------------------------------------------------------//
// end of QueryEnv.hh
//---------------------------------------------------------------------------//
