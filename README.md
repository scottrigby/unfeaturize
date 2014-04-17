```ruby
.-. .-..-. .-.,---.,---.    .--.  _______ .-. .-.,---.  ,-. _____  ,---.
| | | ||  \| || .-'| .-'   / /\ \|__   __|| | | || .-.\ |(|/___  / | .-'
| | | ||   | || `-.| `-.  / /__\ \ )| |   | | | || `-'/ (_)   / /) | `-.
| | | || |\  || .-'| .-'  |  __  |(_) |   | | | ||   (  | |  / /(_)| .-'
| `-')|| | |)|| |  |  `--.| |  |)|  | |   | `-')|| |\ \ | | / /___ |  `--.
`---(_)/(  (_))\|  /( __.'|_|  (_)  `-'   `---(_)|_| \)\`-'(_____/ /( __.’
      (__)   (__) (__)                               (__)         (__)
```

# Presented at NYC Camp 2014
See http://www.nyccamp.org/session/unfeaturizing-distributions.

# Q&A
**Question:** Are Features modules packaged within distributions considered a feature or a bug?  
**Answer:** Yes.

# Background
Since their emergence on the scene in 2009, Features have become a community standard for configuration management in Drupal. They are still an effective way to export, deploy, collaborate on, and track configuration across teams and environments in Drupal 7 (this will be different in Drupal 8, but some of the same challenges below will still need to be addressed).

# Challenges
- Features in distributions prematurely introduce a concept of upstream and downstream configuration in Drupal 7, which is not yet resolved in Drupal 8 CMI
- What happens to downstream overrides when their upstream configurations change? How can we prevent components with dependencies from shitting the bed when the dependency is changed or removed? - We'd need a 3 way diff tool just to resolve the fallout manually

Practically, the problem with packaging features in distributions is that - until we resolve these complex workflow issues - people building on top of those distributions often find themselves locked out of using Features in the way they're useful (when struggling over features component ownership and default states with Features override or CTools alter hooks directly), or else they're forced to fork the distribution: by copying the features in a multisite folder, making their own namespaced variants of components just to sidestep the default ones, or copying the entire Drupal root (including the distribution profile) and editing the base features directly - we're almost not sure which is worse.

# Proposal
We're proposing concurrent solutions for each supported major Drupal version:

- Drupal 8: work on the best path forward in handling issues around upstream and downstream configurations in CMI
- Drupal 7: don't introduce this problem unnecessarily. When it comes to upstream Features in distributions, make an exit strategy. The Unfeaturize module provides an automated way to do this.

# Unfeaturize goals
1. Untether a "feature" module from Features.module.
2. Provide a mechanism to keep original exported component defaults for inserting/reverting, without Features tracking functionality.
3. This will allow new Features to include the default components, without requiring the pains of overriding.
4. Automate as much of this as possible
    - Do not require people implementing distributions to do any manual steps
    - Attempt to limit the manual steps for distribution developers in instituting this change

# Unfeaturize strategy
1. Keep Features default include file and callback structure the same (so downstream features can contribute fixes upstream more easily)
    - We will need custom callbacks that restore defaults to the DB, because Features revert for CTools-style components (views_view etc) deletes the DB row rather than restoring, unlike other non-destructive components (like field_instance). So what we need is our own variation that always does restore (similar to "Features consolidate" https://drupal.org/node/1014522, or "Features Unlink" https://drupal.org/project/ftools, but without the need to do that manually). These hooks can be added to install hooks and possibly available to Drush for a "hard revert" option independent of Features API.
    - Question: should we rename the files and callbacks, to make it clear the omission of features API and info file lines is not accidental?
2. Reuse Features API functions where possible.
